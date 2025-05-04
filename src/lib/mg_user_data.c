/*
 SPDX-License-Identifier: GPL-3.0-or-later
 (c) 2025 Juergen Mang <mail@jcgames.de>
 https://github.com/jcorporation/kebacc
*/

/*! \file
 * \brief Custom mongoose data structure
 */

#include "compile_time.h"
#include "src/lib/mg_user_data.h"

#include "src/lib/mem.h"

#include <string.h>

/**
 * Mallocs and initializes the t_mg_user_data struct
 * @param config Pointer to static config
 * @return struct t_mg_user_data*
 */
struct t_mg_user_data *mg_user_data_new(struct t_config *config) {
    struct t_mg_user_data *mg_user_data = malloc_assert(sizeof(struct t_mg_user_data));
    mg_user_data->config = config;
    mg_user_data->wallbox_conn = NULL;
    mg_user_data->wallbox_status.i.response = strdup("{}");
    mg_user_data->wallbox_status.i.ts = 0;
    mg_user_data->wallbox_status.report1.response = strdup("{}");
    mg_user_data->wallbox_status.report1.ts = 0;
    mg_user_data->wallbox_status.report2.response = strdup("{}");
    mg_user_data->wallbox_status.report2.ts = 0;
    mg_user_data->wallbox_status.report3.response = strdup("{}");
    mg_user_data->wallbox_status.report3.ts = 0;
    return mg_user_data;
}

/**
 * Frees the members of mg_user_data struct and the struct itself
 * @param mg_user_data pointer to mg_user_data struct
 */
void mg_user_data_free(struct t_mg_user_data *mg_user_data) {
    FREE_PTR(mg_user_data->wallbox_status.i.response);
    FREE_PTR(mg_user_data->wallbox_status.report1.response);
    FREE_PTR(mg_user_data->wallbox_status.report2.response);
    FREE_PTR(mg_user_data->wallbox_status.report3.response);
    FREE_PTR(mg_user_data);
}
