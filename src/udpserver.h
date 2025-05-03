/*
 SPDX-License-Identifier: GPL-3.0-or-later
 (c) 2025 Juergen Mang <mail@jcgames.de>
 https://github.com/jcorporation/kebacc
*/

/*! \file
 * \brief Webserver implementation
 */

#ifndef KEBACC_UDPSERVER_H
#define KEBACC_UDPSERVER_H

#include "dist/mongoose/mongoose.h"
#include "src/config.h"
#include "src/mg_user_data.h"

#include <stdbool.h>

bool udpserver_init(struct mg_mgr *mgr, struct t_config *config, struct t_mg_user_data *mg_user_data);
void udpserver_loop(struct mg_mgr *mgr);
void udpserver_free(struct mg_mgr *mgr);

#endif
