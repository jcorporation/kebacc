/*
 SPDX-License-Identifier: GPL-3.0-or-later
 (c) 2025 Juergen Mang <mail@jcgames.de>
 https://github.com/jcorporation/kebacc
*/

/*! \file
 * \brief Command line options handling
 */

#include "compile_time.h"
#include "src/options.h"

#include "src/config.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Options definitions
 */
static struct option long_options[] = {
    {"help",     no_argument,       0, 'h'},
    {"listen",   required_argument, 0, 'l'},
    {"loglevel", required_argument, 0, 'o'},
    {"poll",     required_argument, 0, 'p'},
    {"version",  no_argument,       0, 'v'},
    {"wallbox",  required_argument, 0, 'i'},
    {"workdir",  required_argument, 0, 'w'}
};

/**
 * Prints the command line usage information
 * @param config pointer to config struct
 * @param cmd argv[0] from main function
 */
static void print_usage(struct t_config *config, const char *cmd) {
    (void)fprintf(stderr, "\nUsage: %s [OPTION]...\n\n"
                    "Keba UDP API Client %s\n"
                    "(c) 2025 Juergen Mang <mail@jcgames.de>\n"
                    "https://github.com/jcorporation/kebacc\n\n"
                    "Options:\n"
                    "  -h, --help              Displays this help\n"
                    "  -i, --wallbox <ip>      IP Address of your Keba wallbox\n"
                    "  -l, --listen <uri>      Listen URI (default: %s)\n"
                    "  -o, --loglevel <level>  Syslog loglevel (default: 5 - NOTICE)\n"
                    "  -p, --poll <seconds>    Polling interval in seconds (default: %lu)\n"
                    "  -v, --version           Displays this help\n"
                    "  -w, --workdir <folder>  Working directory (default: %s)\n\n",
        cmd, KEBACC_VERSION, config->listen, (config->poll / 1000), config->workdir);
}

/**
 * Handles the command line arguments
 * @param config pointer to config struct
 * @param argc from main function
 * @param argv from main function
 * @return OPTIONS_RC_INVALID on error
 *         OPTIONS_RC_EXIT if Kebacc should exit
 *         OPTIONS_RC_OK if arguments are parsed successfully
 */
enum handle_options_rc handle_options(struct t_config *config, int argc, char **argv) {
    int n = 0;
    int option_index = 0;
    while ((n = getopt_long(argc, argv, "l:o:vhi:w:", long_options, &option_index)) != -1) {
        switch(n) {
            case 'l':
                free(config->listen);
                config->listen = strdup(optarg);
                break;
            case 'o':
                //TODO
                break;
            case 'v':
            case 'h':
                print_usage(config, argv[0]);
                return OPTIONS_RC_EXIT;
            case 'i': {
                free(config->wallbox_ip);
                config->wallbox_ip = strdup(optarg);
                break;
            }
            case 'p': {
                config->poll = strtoul(optarg, NULL, 10);
                config->poll = config->poll * 1000;
                break;
            }
            case 'w':
                free(config->workdir);
                config->workdir = strdup(optarg);
                break;
            default:
                print_usage(config, argv[0]);
                return OPTIONS_RC_INVALID;
        }
    }
    return OPTIONS_RC_OK;
}
