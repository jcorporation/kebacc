/*
 SPDX-License-Identifier: GPL-3.0-or-later
 (c) 2018-2025 Juergen Mang <mail@jcgames.de>
 https://github.com/jcorporation/kebacc
*/

/*! \file
 * \brief Central kebacc configuration definitions
 */

#ifndef KEBACC_CONFIG_H
#define KEBACC_CONFIG_H

#include "compile_time.h"

/**
 * Static kebacc configuration
 */
struct t_config {
    char *rest_listen;
    char *wallbox_listen;
    char *wallbox_ip;
    char *workdir;
    unsigned long poll;
};

struct t_config *kebacc_config_new(void);
void kebacc_config_free(struct t_config *config);

#endif
