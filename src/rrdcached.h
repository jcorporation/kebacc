/*
 SPDX-License-Identifier: GPL-3.0-or-later
 (c) 2025 Juergen Mang <mail@jcgames.de>
 https://github.com/jcorporation/kebacc
*/

/*! \file
 * \brief Send data to rrdcached
 */

#ifndef KEBACC_RRDCACHED_H
#define KEBACC_RRDCACHED_H

#include "dist/mongoose/mongoose.h"
#include "src/lib/mg_user_data.h"

enum wallbox_report {
    WALLBOX_REPORT2,
    WALLBOX_REPORT3
};

void send_data_to_rrdcached(struct mg_mgr *mgr, enum wallbox_report report, struct t_wallbox_response *data);

#endif
