/*
 SPDX-License-Identifier: GPL-3.0-or-later
 (c) 2018-2025 Juergen Mang <mail@jcgames.de>
 https://github.com/jcorporation/kebacc
*/

/*! \file
 * \brief Wallbox data handler
 */

#ifndef KEBACC_DATA_HANDLER_H
#define KEBACC_DATA_HANDLER_H

#include "compile_time.h"

#include "dist/mongoose/mongoose.h"

void data_handler(struct mg_connection *nc);

#endif
