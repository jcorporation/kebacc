/*
 SPDX-License-Identifier: GPL-3.0-or-later
 (c) 2025 Juergen Mang <mail@jcgames.de>
 https://github.com/jcorporation/kebacc
*/

#ifndef KEBACC_LOG_H
#define KEBACC_LOG_H

#include <stdbool.h>
#include <syslog.h>

/**
 * Macros for logging
 */
#define KEBACC_LOG_EMERG(...) kebacc_log(LOG_EMERG, __VA_ARGS__)
#define KEBACC_LOG_ALERT(...) kebacc_log(LOG_ALERT, __VA_ARGS__)
#define KEBACC_LOG_CRIT(...) kebacc_log(LOG_CRIT, __VA_ARGS__)
#define KEBACC_LOG_ERROR(...) kebacc_log(LOG_ERR, __VA_ARGS__)
#define KEBACC_LOG_WARN(...) kebacc_log(LOG_WARNING, __VA_ARGS__)
#define KEBACC_LOG_NOTICE(...) kebacc_log(LOG_NOTICE, __VA_ARGS__)
#define KEBACC_LOG_INFO(...) kebacc_log(LOG_INFO, __VA_ARGS__)
#define KEBACC_LOG_DEBUG(...) kebacc_log(LOG_DEBUG, __VA_ARGS__)
#define KEBACC_LOG_ERRNO(ERRNUM) kebacc_log_errno(ERRNUM)

/**
 * Global log variables
 */
extern _Atomic int loglevel;
extern bool log_on_tty;

const char *get_loglevel_name(int level);
void set_loglevel(int level);

void kebacc_log_errno(int errnum);
void kebacc_log(int level, const char *fmt, ...)
    __attribute__ ((format (printf, 2, 3)));

#endif
