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

#include "src/config.h"

/**
 * Struct for mg_mgr userdata
 */
struct t_mg_user_data {
    struct t_config *config;         //!< Pointer to kebacc configuration
};

void mg_user_data_free(struct t_mg_user_data *mg_user_data);

#endif
