/*
 SPDX-License-Identifier: GPL-3.0-or-later
 (c) 2018-2025 Juergen Mang <mail@jcgames.de>
 https://github.com/jcorporation/kebacc
*/

/*! \file
 * \brief Configuration handling
 */

#include "compile_time.h"
#include "src/lib/config.h"

#include "src/lib/mem.h"

#include <string.h>

/**
 * Mallocs and initializes the config struct
 * @return config pointer to config struct
 */
struct t_config *kebacc_config_new(void) {
    struct t_config *config = malloc_assert(sizeof(struct t_config));
    config->rest_listen = strdup(REST_LISTEN);
    config->wallbox_listen = strdup(WALLBOX_LISTEN);
    config->wallbox_ip = strdup(WALLBOX_IP);
    config->workdir = strdup(WORKDIR);
    config->poll = POLL_INTERVAL;
    return config;
}

/**
 * Frees the config struct
 * @param config pointer to config struct
 */
void kebacc_config_free(struct t_config *config) {
    FREE_PTR(config->rest_listen);
    FREE_PTR(config->wallbox_listen);
    FREE_PTR(config->wallbox_ip);
    FREE_PTR(config->workdir);
    FREE_PTR(config);
}
