/*
 SPDX-License-Identifier: GPL-3.0-or-later
 (c) 2025 Juergen Mang <mail@jcgames.de>
 https://github.com/jcorporation/kebacc
*/

/*! \file
 * \brief Wallbox data handler
 */

#include "compile_time.h"
#include "src/data_handler.h"

#include "src/lib/filehandler.h"
#include "src/lib/log.h"
#include "src/lib/mem.h"
#include "src/lib/mg_user_data.h"

#include "src/rrdcached.h"

// Private definitions

static void setdata(struct t_wallbox_response *response, char *data);

// Public functions

/**
 * Wallbox data handler.
 * Stores the data in the wallbox_status struct and saves it to a file in the workdir.
 * @param mg_user_data Pointer to t_mg_user_data struct
 * @param buf Pointer to buffer with received data 
 */
bool data_handler(struct mg_connection *nc) {
    struct t_mg_user_data *mg_user_data = (struct t_mg_user_data *) nc->mgr->userdata;
    struct t_config *config = mg_user_data->config;
    // Copy received data in NULL-terminated string while removing white space and newline characters.
    char *data = malloc_assert(nc->recv.len + 1);
    size_t data_len = 0;
    for (size_t i = 0; i < nc->recv.len; i++) {
        if (isblank(nc->recv.buf[i]) == false &&
            nc->recv.buf[i] != '\r' &&
            nc->recv.buf[i] != '\n')
        {
            data[data_len++] = (char)nc->recv.buf[i];
        }
    }
    data[data_len] = '\0';
    data_len++;
    KEBACC_LOG_DEBUG("Wallbox: Received data: %s", data);
    if (data_len < 8) {
        KEBACC_LOG_ERROR("Wallbox: Invalid data received.");
        return false;
    }
    switch(data[7]) {
        case 'r':
            setdata(&mg_user_data->wallbox_status.i, data);
            write_data_to_file(config->workdir, "wallbox_i.txt", data, data_len);
            return true;
        case '1':
            setdata(&mg_user_data->wallbox_status.report1, data);
            write_data_to_file(config->workdir, "wallbox_report1.json", data, data_len);
            return true;
        case '2':
            setdata(&mg_user_data->wallbox_status.report2, data);
            write_data_to_file(config->workdir, "wallbox_report2.json", data, data_len);
            if (config->rrdcached_uri[0] != '\0') {
                send_data_to_rrdcached(nc->mgr, WALLBOX_REPORT2, &mg_user_data->wallbox_status.report2);
            }
            return true;
        case '3':
            setdata(&mg_user_data->wallbox_status.report3, data);
            write_data_to_file(config->workdir, "wallbox_report3.json", data, data_len);
            if (config->rrdcached_uri[0] != '\0') {
                send_data_to_rrdcached(nc->mgr, WALLBOX_REPORT3, &mg_user_data->wallbox_status.report3);
            }
            return true;
        default:
            if (strncmp(data, "{\"Epres\":", 8) == 0) {
                //Energy transferred in the current charging session in 0.1 Wh
            }
            else {
                KEBACC_LOG_WARN("Wallbox: Unhandled data received: %s", data);
            }
    }
    return false;
}

// Private functions

/**
 * Saves the wallbox data internally. This data is used in the REST API implementation.
 * @param response Pointer to t_wallbox_response struct
 * @param data Data to store
 */
static void setdata(struct t_wallbox_response *response, char *data) {
    FREE_PTR(response->response);
    response->response = data;
    response->ts = time(NULL);
}
