/*
 SPDX-License-Identifier: GPL-3.0-or-later
 (c) 2025 Juergen Mang <mail@jcgames.de>
 https://github.com/jcorporation/kebacc
*/

/*! \file
 * \brief UDP client implementation for the Keba Wallbox
 */

#include "compile_time.h"
#include "src/wallbox.h"

#include "src/data_handler.h"
#include "src/lib/log.h"
#include "src/lib/mg_user_data.h"

 /**
 * Generic timer handler to poll data from the wallbox
 * @param arg Pointer to mongoose mgr
 * @param arg Command to send
 */
void timer_wallbox_send_cmd(void *arg, const char *cmd) {
    struct mg_mgr *mgr = (struct mg_mgr *) arg;
    struct t_mg_user_data *mg_user_data = (struct t_mg_user_data *) mgr->userdata;
    if (mg_user_data->wallbox_conn != NULL) {
        KEBACC_LOG_DEBUG("Sending \"%s\"", cmd);
        mg_send(mg_user_data->wallbox_conn, cmd, strlen(cmd));
    }
    else {
        // This should not appear
        KEBACC_LOG_ERROR("No wallbox connection available.");
    }
}

/**
 * Sends "report 1" to the wallbox and schedules the "report 3" timer.
 * @param arg Pointer to mongoose mgr
 */
void timer_wallbox_send_report_1(void *arg) {
    struct mg_mgr *mgr = (struct mg_mgr *) arg;
    struct t_mg_user_data *mg_user_data = (struct t_mg_user_data *) mgr->userdata;
    struct t_config *config = mg_user_data->config;
    timer_wallbox_send_cmd(arg, "report 1");
    // Add timer for "report 3".
    mg_timer_add(mgr, (unsigned)config->poll, MG_TIMER_REPEAT, timer_wallbox_send_report_3, mgr);
}

/**
 * Sends "report 2" to the wallbox
 * @param arg Pointer to mongoose mgr
 */
void timer_wallbox_send_report_2(void *arg) {
    timer_wallbox_send_cmd(arg, "report 2");
}

/**
 * Sends "report 3" to the wallbox
 * @param arg Pointer to mongoose mgr
 */
void timer_wallbox_send_report_3(void *arg) {
    timer_wallbox_send_cmd(arg, "report 3");
}

/**
 * Central wallbox event handler
 * @param nc mongoose connection
 * @param ev connection event
 * @param ev_data event data
 */
void ev_handler_wallbox(struct mg_connection *nc, int ev, void *ev_data) {
    (void) ev_data; //unused
    struct t_mg_user_data *mg_user_data = (struct t_mg_user_data *) nc->mgr->userdata;
    struct t_config *config = mg_user_data->config;
    if (ev == MG_EV_OPEN && nc->is_listening) {
        // Remember wallbox listening connection
        mg_user_data->wallbox_conn = nc;
        // Set wallbox uri
        // We are sending the udp packets from the listening connection as source, because
        // we must use 7090 as source port. The listening connection has no remote peer,
        // we set it manually. A bit low level, but it works.
        // https://github.com/cesanta/mongoose/discussions/3124
        nc->rem.port = mg_htons(WALLBOX_API_PORT);
        mg_aton(mg_str(config->wallbox_ip), (struct mg_addr *)nc->rem.ip);
        // Send initial request
        KEBACC_LOG_DEBUG("Sending \"i\"");
        mg_send(nc, "i", 1);
    }
    else if (ev == MG_EV_READ) {
        data_handler(nc);
        mg_iobuf_del(&nc->recv, 0, nc->recv.len);
    }
}
