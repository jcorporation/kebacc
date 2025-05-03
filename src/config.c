/*
 SPDX-License-Identifier: GPL-3.0-or-later
 (c) 2018-2025 Juergen Mang <mail@jcgames.de>
 https://github.com/jcorporation/kebacc
*/

/*! \file
 * \brief Configuration handling
 */

#include "compile_time.h"
#include "src/config.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

/**
 * Mallocs and initializes the config struct
 * @return config pointer to config struct
 */
struct t_config *kebacc_config_new(void) {
    struct t_config *config = malloc(sizeof(struct t_config));
    assert(config);
    config->listen = strdup("udp://0.0.0.0:7090");
    config->wallbox_ip = strdup("192.168.93.199");
    config->workdir = strdup("/var/lib/kebacc");
    config->poll = 60000;
    return config;
}


/**
 * Frees the config struct
 * @param config pointer to config struct
 */
void kebacc_config_free(struct t_config *config) {
    free(config->listen);
    free(config->wallbox_ip);
    free(config->workdir);
    free(config);
}
