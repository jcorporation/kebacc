/*
 SPDX-License-Identifier: GPL-3.0-or-later
 (c) 2018-2025 Juergen Mang <mail@jcgames.de>
 https://github.com/jcorporation/kebacc
*/

#include "compile_time.h"

#include "dist/mongoose/mongoose.h"

#include "src/config.h"
#include "src/log.h"
#include "src/mg_user_data.h"
#include "src/options.h"
#include "src/udpserver.h"

#include <errno.h>

//signal handler
sig_atomic_t s_signal_received;

/**
 * Signal handler
 * @param sig_num the signal to handle
 */
static void kebacc_signal_handler(int sig_num) {
    switch(sig_num) {
        case SIGTERM:
        case SIGINT: {
            KEBACC_LOG_NOTICE("Signal \"%s\" received, exiting", (sig_num == SIGTERM ? "SIGTERM" : "SIGINT"));
            //Set loop end condition for threads
            s_signal_received = sig_num;
            break;
        }
        default: {
            //Other signals are not handled
        }
    }
}

/**
 * Sets the kebacc_signal_handler for the given signal
 * @param sig_num signal to handle
 * @return true on success, else false
 */
static bool set_signal_handler(int sig_num) {
    struct sigaction sa;
    sa.sa_handler = kebacc_signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART; // Restart functions if interrupted by handler
    if (sigaction(sig_num, &sa, NULL) == -1) {
        return false;
    }
    return true;
}

/**
 * The main function.
 * @param argc number of command line arguments
 * @param argv char array of the command line arguments
 * @return 0 on success
 */
int main(int argc, char **argv) {
    //set logging states
    log_on_tty = isatty(fileno(stdout));
    #ifdef KEBACC_DEBUG
        set_loglevel(LOG_DEBUG);
        KEBACC_LOG_NOTICE("Debug build is running");
    #else
        set_loglevel(LOG_NOTICE);
    #endif

    //only owner should have rw access
    umask(0077);

    //central data structures
    s_signal_received = 0;
    struct t_mg_user_data *mg_user_data = NULL;
    struct mg_mgr *mgr = NULL;
    int rc = EXIT_FAILURE;
    struct t_config *config = kebacc_config_new();

    //command line option
    enum handle_options_rc options_rc = handle_options(config, argc, argv);
    switch(options_rc) {
        case OPTIONS_RC_INVALID:
            //invalid option or error
            loglevel = LOG_ERR;
            goto cleanup;
        case OPTIONS_RC_EXIT:
            //valid option and exit
            loglevel = LOG_ERR;
            rc = EXIT_SUCCESS;
            goto cleanup;
        case OPTIONS_RC_OK:
            //continue
            break;
    }

    //go into workdir
    errno = 0;
    if (chdir(config->workdir) != 0) {
        KEBACC_LOG_ERROR("Can not change directory to \"%s\"", config->workdir);
        KEBACC_LOG_ERRNO(errno);
        goto cleanup;
    }

    KEBACC_LOG_NOTICE("Starting kebacc %s", KEBACC_VERSION);
    KEBACC_LOG_INFO("Mongoose %s", MG_VERSION);

    //set output buffers
    if (setvbuf(stdout, NULL, _IOLBF, 0) != 0 ||
        setvbuf(stderr, NULL, _IOLBF, 0) != 0)
    {
        KEBACC_LOG_EMERG("Could not set stdout and stderr buffer");
        goto cleanup;
    }

    //set signal handler
    if (set_signal_handler(SIGTERM) == false ||
        set_signal_handler(SIGINT) == false)
    {
        KEBACC_LOG_EMERG("Could not set signal handler for SIGTERM and SIGINT");
        goto cleanup;
    }

    //init udpserver
    mgr = malloc(sizeof(struct mg_mgr));
    assert(mgr);
    mg_user_data = malloc(sizeof(struct t_mg_user_data));
    assert(mg_user_data);
    if (udpserver_init(mgr, config, mg_user_data) == false) {
        goto cleanup;
    }

    //Run the udpserver
    udpserver_loop(mgr);
    rc = EXIT_SUCCESS;

    //Try to cleanup all
    cleanup:

    kebacc_config_free(config);

    if (mgr != NULL) {
        udpserver_free(mgr);
    }
    if (mg_user_data != NULL) {
        mg_user_data_free(mg_user_data);
    }
    if (rc == EXIT_SUCCESS) {
        printf("Exiting gracefully\n");
    }
    else {
        printf("Exiting erroneous\n");
    }

    return rc;
}
