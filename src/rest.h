/*
 SPDX-License-Identifier: GPL-3.0-or-later
 (c) 2025 Juergen Mang <mail@jcgames.de>
 https://github.com/jcorporation/kebacc
*/

/*! \file
 * \brief REST API implementation
 */

#ifndef KEBACC_REST_H
#define KEBACC_REST_H

#include "dist/mongoose/mongoose.h"

void ev_handler_rest(struct mg_connection *nc, int ev, void *ev_data);

#endif
