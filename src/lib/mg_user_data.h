/*
 SPDX-License-Identifier: GPL-3.0-or-later
 (c) 2025 Juergen Mang <mail@jcgames.de>
 https://github.com/jcorporation/kebacc
*/

/*! \file
 * \brief Custom mongoose data structure
 */

#ifndef KEBACC_MG_USER_DATA_H
#define KEBACC_MG_USER_DATA_H

#include "src/lib/config.h"

#include <time.h>

/**
 * Struct for a wallbox response
 */
struct t_wallbox_response {
    char *response;  //!< Wallbox response
    time_t ts;       //!< Timestamp
};

/**
 * Struct for polled wallbox status
 */
struct t_wallbox_status {
    struct t_wallbox_response i;        //!< Last response to "i" command
    struct t_wallbox_response report1;  //!< Last response to "report 1" command
    struct t_wallbox_response report2;  //!< Last response to "report 2" command
    struct t_wallbox_response report3;  //!< Last response to "report 3" command
};

/**
 * Struct for mg_mgr userdata
 */
struct t_mg_user_data {
    struct t_config *config;                  //!< Pointer to Kebacc configuration
    struct t_wallbox_status wallbox_status;   //!< Last polled wallbox status
    struct mg_connection *wallbox_conn;       //!< Pointer to UDP listener for wallbox communication
    struct mg_connection *rrdached_conn;      //!< Pointer to the rrdached connection
};

struct t_mg_user_data *mg_user_data_new(struct t_config *config);
void mg_user_data_free(struct t_mg_user_data *mg_user_data);

#endif
