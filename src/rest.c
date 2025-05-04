/*
 SPDX-License-Identifier: GPL-3.0-or-later
 (c) 2025 Juergen Mang <mail@jcgames.de>
 https://github.com/jcorporation/kebacc
*/

/*! \file
 * \brief REST API implementation
 */

#include "compile_time.h"
#include "src/rest.h"

#include "src/lib/log.h"
#include "src/lib/mem.h"
#include "src/lib/mg_user_data.h"

// Private definitions
static bool rest_get(struct mg_connection *nc, struct mg_http_message *hm);

static const char *response_headers = "Content-Type: application/json\r\n";

// Public functions

/**
 * Central REST event handler
 * @param nc mongoose connection
 * @param ev connection event
 * @param ev_data event data
 */
void ev_handler_rest(struct mg_connection *nc, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;
        bool rc = false;
        KEBACC_LOG_DEBUG("HTTP request to %.*s", (int)hm->uri.len, hm->uri.buf);
        if (mg_strcmp(hm->method, mg_str("GET")) == 0) {
            rc = rest_get(nc, hm);
        }
        // Other methods are not yet implemented
        if (rc == false) {
            // Request was not handled
            mg_http_reply(nc, 200, response_headers, "%s\n", "{\"error\":\"Not implemented\"}");
        }
    }
}

// Private functions

/**
 * Handler for GET method
 * @param nc Mongoose connection
 * @param hm HTTP request
 * @return true if request was handled, else false
 */
static bool rest_get(struct mg_connection *nc, struct mg_http_message *hm) {
    struct t_mg_user_data *mg_user_data = (struct t_mg_user_data *) nc->mgr->userdata;
    if (mg_strcmp(hm->uri, mg_str("/status")) == 0) {
        char *json = mg_mprintf("{"
                "\"i\":{\"ts\":%lld,\"response\":{%s}},"
                "\"report1\":{\"ts\":%lld,\"response\":%s},"
                "\"report2\":{\"ts\":%lld,\"response\":%s},"
                "\"report3\":{\"ts\":%lld,\"response\":%s}"
            "}",
            (long long)mg_user_data->wallbox_status.i.ts, mg_user_data->wallbox_status.i.response,
            (long long)mg_user_data->wallbox_status.report1.ts, mg_user_data->wallbox_status.report1.response,
            (long long)mg_user_data->wallbox_status.report2.ts, mg_user_data->wallbox_status.report2.response,
            (long long)mg_user_data->wallbox_status.report3.ts, mg_user_data->wallbox_status.report3.response);
        mg_http_reply(nc, 200, response_headers, "%s", json);
        FREE_PTR(json);
        return true;
    }
    if (mg_strcmp(hm->uri, mg_str("/status/i")) == 0) {
        char *json = mg_mprintf("{\"ts\": %lld,\"i\":{%s}}",
            (long long)mg_user_data->wallbox_status.i.ts, mg_user_data->wallbox_status.i.response);
        mg_http_reply(nc, 200, response_headers, "%s", json);
        FREE_PTR(json);
        return true;
    }
    if (mg_strcmp(hm->uri, mg_str("/status/report1")) == 0) {
        char *json = mg_mprintf("{\"ts\": %lld,\"report1\":%s}",
            (long long)mg_user_data->wallbox_status.report1.ts, mg_user_data->wallbox_status.report1.response);
        mg_http_reply(nc, 200, response_headers, "%s", json);
        FREE_PTR(json);
        return true;
    }
    if (mg_strcmp(hm->uri, mg_str("/status/report2")) == 0) {
        char *json = mg_mprintf("{\"ts\": %lld,\"report2\":%s}",
            (long long)mg_user_data->wallbox_status.report2.ts, mg_user_data->wallbox_status.report2.response);
        mg_http_reply(nc, 200, response_headers, "%s", json);
        FREE_PTR(json);
        return true;
    }
    if (mg_strcmp(hm->uri, mg_str("/status/report3")) == 0) {
        char *json = mg_mprintf("{\"ts\": %lld,\"report3\":%s}",
            (long long)mg_user_data->wallbox_status.report3.ts, mg_user_data->wallbox_status.report3.response);
        mg_http_reply(nc, 200, response_headers, "%s", json);
        FREE_PTR(json);
        return true;
    }
    return false;
}
