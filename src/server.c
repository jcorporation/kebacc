/*
 SPDX-License-Identifier: GPL-3.0-or-later
 (c) 2025 Juergen Mang <mail@jcgames.de>
 https://github.com/jcorporation/kebacc
*/

/*! \file
 * \brief Mongoose server implementation
 */

#include "compile_time.h"
#include "src/server.h"

#include "src/lib/log.h"
#include "src/lib/mem.h"
#include "src/lib/mg_user_data.h"
#include "src/rest.h"
#include "src/wallbox.h"

#include <stdbool.h>

// Private definitions

static void mongoose_log(char ch, void *param);

// Public functions

/**
 * Initializes the udpserver
 * @param mgr Already allocated Mongoose mgr
 * @param config Pointer to Kebacc config
 * @param mg_user_data t_mg_user_data to set
 * @return true on success, else false
 */
bool mongoose_init(struct mg_mgr *mgr, struct t_config *config, struct t_mg_user_data *mg_user_data) {
    // Init mongoose mgr
    mg_mgr_init(mgr);
    mgr->userdata = mg_user_data;

    struct mg_connection *nc_wallbox = mg_listen(mgr, config->wallbox_listen, ev_handler_wallbox, NULL);
    if (nc_wallbox == NULL) {
        KEBACC_LOG_EMERG("Server: Can't bind to %s", config->wallbox_listen);
        return false;
    }
    KEBACC_LOG_NOTICE("Server: Listening on %s for wallbox responses", config->wallbox_listen);

    struct mg_connection *nc_rest = mg_http_listen(mgr, config->rest_listen, ev_handler_rest, NULL);
    if (nc_rest == NULL) {
        KEBACC_LOG_EMERG("Server: Can't bind to %s", config->rest_listen);
        return false;
    }
    KEBACC_LOG_NOTICE("Server: Listening on %s for REST requests", config->rest_listen);

    // Add timer to poll data from the wallbox.
    // We fetch "report 1" only once, 10 seconds after start.
    // This timer schedules the "report 3" timer for the poll interval.
    mg_timer_add(mgr, 10000, MG_TIMER_ONCE, timer_wallbox_send_report_1, mgr);
    // We fetch "report 2" at poll interval.
    mg_timer_add(mgr, (unsigned)config->poll, MG_TIMER_REPEAT, timer_wallbox_send_report_2, mgr);

    return true;
}

/**
 * Frees the mongoose mgr
 * @param mgr mongoose mgr to free
 */
void mongoose_free(struct mg_mgr *mgr) {
    mg_mgr_free(mgr);
    FREE_PTR(mgr);
}

/**
 * UDP Server polling
 * @pram arg_mgr void pointer to mongoose mgr
 */
void mongoose_loop(struct mg_mgr *mgr) {
    //struct t_mg_user_data *mg_user_data = (struct t_mg_user_data *) mgr->userdata;
    //struct t_config *config = mg_user_data->config;
    // Set mongoose loglevel to error
    mg_log_set(1);
    mg_log_set_fn(mongoose_log, NULL);
    for (;;) {
        // Webserver polling
        mg_mgr_poll(mgr, 10000);
    }
    KEBACC_LOG_INFO("Server: Stopping");
}

/**
 * Mongoose logging function
 * @param ch character to log
 * @param param
 */
static void mongoose_log(char ch, void *param) {
    static char buf[256];
    static size_t len;
    buf[len++] = ch;
    if (ch == '\n' ||
        len >= sizeof(buf))
    {
        if (ch == '\n') {
            // Remove newline
            len--;
        }
        // We log only mongoose errors
        KEBACC_LOG_ERROR("%.*s", (int) len, buf); // Send logs
        len = 0;
    }
    (void)param;
}
