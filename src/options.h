/*
 SPDX-License-Identifier: GPL-3.0-or-later
 (c) 2018-2025 Juergen Mang <mail@jcgames.de>
 https://github.com/jcorporation/kebacc
*/

/*! \file
 * \brief Command line options handling
 */

#ifndef KEBACC_OPTIONS_H
#define KEBACC_OPTIONS_H

#include "src/config.h"

#include <stdbool.h>

/**
 * Return codes for options handler
 */
enum handle_options_rc {
    OPTIONS_RC_INVALID = -1,
    OPTIONS_RC_OK = 0,
    OPTIONS_RC_EXIT = 1
};

enum handle_options_rc handle_options(struct t_config *config, int argc, char **argv);

#endif
