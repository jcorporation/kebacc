/*
 SPDX-License-Identifier: GPL-3.0-or-later
 (c) 2018-2025 Juergen Mang <mail@jcgames.de>
 https://github.com/jcorporation/kebacc
*/

/*! \file
 * \brief Custom mongoose data structure
 */

#include "compile_time.h"
#include "src/mg_user_data.h"

#include <stdlib.h>

/**
 * Frees the members of mg_user_data struct and the struct itself
 * @param mg_user_data pointer to mg_user_data struct
 */
void mg_user_data_free(struct t_mg_user_data *mg_user_data) {
    free(mg_user_data);
}
