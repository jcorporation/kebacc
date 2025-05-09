/*
 SPDX-License-Identifier: GPL-3.0-or-later
 (c) 2025 Juergen Mang <mail@jcgames.de>
 https://github.com/jcorporation/kebacc
*/

/*! \file
 * \brief Send data to RRDCached
 */

#include "compile_time.h"
#include "src/rrdcached.h"

#include "src/lib/log.h"
#include "src/lib/mem.h"
#include "src/lib/mg_user_data.h"

// Private definitions

static void ev_handler_rrdcached(struct mg_connection *nc, int ev, void *ev_data);
static char *json_parse_report(enum wallbox_report report, struct t_wallbox_response *data);
static char *json_parse_report2(struct t_wallbox_response *data);
static char *json_parse_report3(struct t_wallbox_response *data);

// Public functions

/**
 * Sends the data to a RRDCached instance
 * @param mgr Mongoose mgr
 * @param report Report to send
 * @param data Data to parse
 * @param data_len Data Length
 */
void send_data_to_rrdcached(struct mg_mgr *mgr, enum wallbox_report report, struct t_wallbox_response *data) {
    struct t_mg_user_data *mg_user_data = (struct t_mg_user_data *) mgr->userdata;
    struct t_config *config = mg_user_data->config;
    // Parse JSON and create values for rrdcached
    char *line = json_parse_report(report, data);
    if (line == NULL) {
        KEBACC_LOG_ERROR("RRDCached: JSON parsing failure");
        return;
    }
    KEBACC_LOG_DEBUG("RRDCached: Sending %s", line);
    // Create connection to rrdcached or send data if connected
    if (mg_user_data->rrdached_conn == NULL) {
        mg_user_data->rrdached_conn = mg_connect(mgr, config->rrdcached_uri, ev_handler_rrdcached, line);
        if (mg_user_data->rrdached_conn == NULL) {
            KEBACC_LOG_ERROR("RRDCached: Error creating connection to: %s", config->rrdcached_uri);
            FREE_PTR(line);
        }
    }
    else {
        mg_printf(mg_user_data->rrdached_conn, "%s\n", line);
        FREE_PTR(line);
    }
}

// Private functions

/**
 * Central RRDCached connection handler
 * @param nc mongoose connection
 * @param ev connection event
 * @param ev_data event data
 */
static void ev_handler_rrdcached(struct mg_connection *nc, int ev, void *ev_data) {
    struct t_mg_user_data *mg_user_data = (struct t_mg_user_data *) nc->mgr->userdata;
    (void) ev_data; //unused
    if (ev == MG_EV_CONNECT) {
        // Send the data with a newline appended
        char *line =(char *)nc->fn_data;
        mg_printf(nc, "%s\n", line);
        FREE_PTR(line);
        nc->fn_data = NULL;
    }
    else if (ev == MG_EV_READ) {
        // Remove newline
        if (nc->recv.len > 2) {
            nc->recv.buf[nc->recv.len - 2] = '\0';
        }
        if (nc->recv.len == 0 ||
            nc->recv.buf[0] != '0')
        {
            KEBACC_LOG_ERROR("RRDCached: Update failed: %.*s", (int)nc->recv.len, nc->recv.buf);
        }
        else {
            KEBACC_LOG_DEBUG("RRDCached: Response: %.*s", (int)nc->recv.len, nc->recv.buf);
        }
        mg_iobuf_del(&nc->recv, 0, nc->recv.len);
    }
    else if (ev == MG_EV_ERROR) {
        KEBACC_LOG_ERROR("RRDCached: Connection error occurred");
    }
    else if (ev == MG_EV_CLOSE) {
        mg_user_data->rrdached_conn = NULL;
    }
}

/**
 * Switch for report parsing
 * @param report Wallbox report
 * @param data Data to parse
 * @return char* rrd update line to send to rrdcached
 */
static char *json_parse_report(enum wallbox_report report, struct t_wallbox_response *data) {
    switch(report) {
        case WALLBOX_REPORT2: return json_parse_report2(data);
        case WALLBOX_REPORT3: return json_parse_report3(data);
    }
    return NULL;
}

/**
 * Parses the "report 2" response from the wallbox.
 * @param data Data to parse
 * @return char* rrd update line to send to rrdcached or NULL on error
 */
static char *json_parse_report2(struct t_wallbox_response *data) {
    double state;
    double maxcurr;
    struct mg_str json = mg_str(data->response); 
    if (mg_json_get_num(json, "$.State", &state) == true &&
        mg_json_get_num(json, "$.Maxcurr", &maxcurr) == true)
    {
        return mg_mprintf("UPDATE %s %lld:%f:%f",
            RRD_REPORT2, (long long)data->ts, state, maxcurr);
    }
    return NULL;
}

/**
 * Parses the "report 3" response from the wallbox.
 * @param data Data to parse
 * @return char* rrd update line to send to rrdcached or NULL on error
 */
static char *json_parse_report3(struct t_wallbox_response *data) {
    double p;
    struct mg_str json = mg_str(data->response); 
    if (mg_json_get_num(json, "$.P", &p) == true) {
        return mg_mprintf("UPDATE %s %lld:%f",
            RRD_REPORT3, (long long)data->ts, p);
    }
    return NULL;
}
