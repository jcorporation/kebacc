/*
 SPDX-License-Identifier: GPL-3.0-or-later
 (c) 2025 Juergen Mang <mail@jcgames.de>
 https://github.com/jcorporation/kebacc
*/

/*! \file
 * \brief UDP server implementation
 */

#include "compile_time.h"
#include "src/udpserver.h"

#include "src/log.h"
#include "src/mg_user_data.h"

#include <inttypes.h>
#include <libgen.h>
#include <stdbool.h>

// Private definitions

static void timer_udp_send(void *arg);
static void ev_handler_udpserver(struct mg_connection *nc, int ev, void *ev_data);
static void mongoose_log(char ch, void *param);

// Public functions

/**
 * Initializes the udpserver
 * @param mgr mongoose mgr
 * @param config pointer to kebacc config
 * @param mg_user_data already allocated t_mg_user_data to populate
 * @return true on success, else false
 */
bool udpserver_init(struct mg_mgr *mgr, struct t_config *config, struct t_mg_user_data *mg_user_data) {
    // Initialize mgr user_data, malloced in main.c
    mg_user_data->config = config;

    // Init mongoose mgr
    mg_mgr_init(mgr);
    mgr->userdata = mg_user_data;

    struct mg_connection *nc_udp_server = mg_listen(mgr, config->listen, ev_handler_udpserver, NULL);
    if (nc_udp_server == NULL) {
        KEBACC_LOG_EMERG("Can't bind to %s", config->listen);
        return false;
    }
    KEBACC_LOG_NOTICE("Listening on %s", config->listen);

    // Add timer for polling data from the wallbox
    mg_timer_add(mgr, (unsigned)config->poll, MG_TIMER_REPEAT, timer_udp_send, mgr);

    return true;
}

/**
 * Frees the mongoose mgr
 * @param mgr mongoose mgr to free
 * @return NULL
 */
void udpserver_free(struct mg_mgr *mgr) {
    mg_mgr_free(mgr);
    free(mgr);
}

/**
 * UDP Server polling
 * @param arg_mgr void pointer to mongoose mgr
 * @return NULL
 */
void udpserver_loop(struct mg_mgr *mgr) {
    struct t_mg_user_data *mg_user_data = (struct t_mg_user_data *) mgr->userdata;
    struct t_config *config = mg_user_data->config;
    // Set mongoose loglevel to error
    mg_log_set(1);
    mg_log_set_fn(mongoose_log, NULL);
    while (s_signal_received == 0) {
        // Webserver polling
        mg_mgr_poll(mgr, (int)config->poll);
    }
    KEBACC_LOG_INFO("Stopping udpserver");
}

/**
 * Private functions
 */

/**
 * Timer handler to get data from the wallbox
 * @param arg Pointer to mongoose mgr
 */
static void timer_udp_send(void *arg) {
    struct mg_mgr *mgr = (struct mg_mgr *) arg;
    //struct t_mg_user_data *mg_user_data = (struct t_mg_user_data *) mgr->userdata;
    //struct t_config *config = mg_user_data->config;
    const char *cmd = "report 1";
    KEBACC_LOG_DEBUG("Sending \"%s\"", cmd);
    mgr->conns->data[0] = '1';
    mg_send(mgr->conns, cmd, strlen(cmd));
}

/**
 * Central udpserver event handler
 * @param nc mongoose connection
 * @param ev connection event
 * @param ev_data event data
 */
static void ev_handler_udpserver(struct mg_connection *nc, int ev, void *ev_data) {
    (void) ev_data; //unused
    struct t_mg_user_data *mg_user_data = (struct t_mg_user_data *) nc->mgr->userdata;
    struct t_config *config = mg_user_data->config;
    if (ev == MG_EV_OPEN && nc->is_listening) {
        //set wallbox uri
        nc->rem.port = mg_htons(KEBA_API_PORT);
        mg_aton(mg_str(config->wallbox_ip), (struct mg_addr *)nc->rem.ip);
        // Send initial request
        KEBACC_LOG_DEBUG("Sending \"i\"");
        nc->data[0] = 'i';
        mg_send(nc, "i", 1);
    }
    else if (ev == MG_EV_READ) {
        KEBACC_LOG_DEBUG("Received response for \"%c\": %.*s", nc->data[0], (int)nc->recv.len, nc->recv.buf);
        nc->data[0] = '-';
        mg_iobuf_del(&nc->recv, 0, nc->recv.len);
    }
}

/**
 * Mongoose logging function
 * @param ch character to log
 * @param param
 */
static void mongoose_log(char ch, void *param) {
    static char buf[256];
    static size_t len;
    buf[len++] = ch;
    if (ch == '\n' ||
        len >= sizeof(buf))
    {
        if (ch == '\n') {
            // Remove newline
            len--;
        }
        // We log only mongoose errors
        KEBACC_LOG_ERROR("%.*s", (int) len, buf); // Send logs
        len = 0;
    }
    (void)param;
}
