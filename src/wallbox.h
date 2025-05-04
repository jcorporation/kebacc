/*
 SPDX-License-Identifier: GPL-3.0-or-later
 (c) 2025 Juergen Mang <mail@jcgames.de>
 https://github.com/jcorporation/kebacc
*/

/*! \file
 * \brief UDP client implementation for the Keba Wallbox
 */

#ifndef KEBACC_WALLBOX_H
#define KEBACC_WALLBOX_H

#include "dist/mongoose/mongoose.h"

void timer_wallbox_send_cmd(void *arg, const char *cmd);
void timer_wallbox_send_report_1(void *arg);
void timer_wallbox_send_report_2(void *arg);
void timer_wallbox_send_report_3(void *arg);
void ev_handler_wallbox(struct mg_connection *nc, int ev, void *ev_data);

#endif
