/*
 SPDX-License-Identifier: GPL-3.0-or-later
 (c) 2025 Juergen Mang <mail@jcgames.de>
 https://github.com/jcorporation/kebacc
*/

/*! \file
 * \brief Central Kebacc configuration definitions
 */

#ifndef KEBACC_CONFIG_H
#define KEBACC_CONFIG_H

/**
 * Static Kebacc configuration
 */
struct t_config {
    unsigned long poll;    //!< Poll interval for the server loop
    char *rest_listen;     //!< Listening URI for the REST HTTP service
    char *wallbox_listen;  //!< Listening URI for wallbox communication
    char *wallbox_ip;      //!< Wallbox IP
    char *workdir;         //!< Working directory
};

struct t_config *kebacc_config_new(void);
void kebacc_config_free(struct t_config *config);

#endif
