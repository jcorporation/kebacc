/*
 SPDX-License-Identifier: GPL-3.0-or-later
 (c) 2025 Juergen Mang <mail@jcgames.de>
 https://github.com/jcorporation/kebacc
*/

/*! \file
 * \brief File handling
 */

#ifndef KEBACC_FILEHANDLER_H
#define KEBACC_FILEHANDLER_H

#include <stdbool.h>
#include <stdio.h>

bool write_data_to_file(const char *basepath, const char *filename, const char *data, size_t data_len);
FILE *open_tmp_file(char *filepath);
bool rename_tmp_file(FILE *fp, const char *tmp_file);
bool rm_file(const char *filepath);

#endif
