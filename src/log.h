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
#ifdef KEBACC_DEBUG
    #define KEBACC_LOG_EMERG(...) kebacc_log(LOG_EMERG, __FILE__, __LINE__, __VA_ARGS__)
    #define KEBACC_LOG_ALERT(...) kebacc_log(LOG_ALERT, __FILE__, __LINE__, __VA_ARGS__)
    #define KEBACC_LOG_CRIT(...) kebacc_log(LOG_CRIT, __FILE__, __LINE__, __VA_ARGS__)
    #define KEBACC_LOG_ERROR(...) kebacc_log(LOG_ERR, __FILE__, __LINE__, __VA_ARGS__)
    #define KEBACC_LOG_WARN(...) kebacc_log(LOG_WARNING,  __FILE__, __LINE__, __VA_ARGS__)
    #define KEBACC_LOG_NOTICE(...) kebacc_log(LOG_NOTICE, __FILE__, __LINE__, __VA_ARGS__)
    #define KEBACC_LOG_INFO(...) kebacc_log(LOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
    #define KEBACC_LOG_DEBUG(...) kebacc_log(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
    #define KEBACC_LOG_ERRNO(ERRNUM) kebacc_log_errno(__FILE__, __LINE__, ERRNUM)
#else
    /**
     * release build should have no references to build dir
     */
    #define KEBACC_LOG_EMERG(...) kebacc_log(LOG_EMERG, "", __LINE__, __VA_ARGS__)
    #define KEBACC_LOG_ALERT(...) kebacc_log(LOG_ALERT, "", __LINE__, __VA_ARGS__)
    #define KEBACC_LOG_CRIT(...) kebacc_log(LOG_CRIT, "", __LINE__, __VA_ARGS__)
    #define KEBACC_LOG_ERROR(...) kebacc_log(LOG_ERR, "", __LINE__, __VA_ARGS__)
    #define KEBACC_LOG_WARN(...) kebacc_log(LOG_WARNING, "", __LINE__, __VA_ARGS__)
    #define KEBACC_LOG_NOTICE(...) kebacc_log(LOG_NOTICE, "", __LINE__, __VA_ARGS__)
    #define KEBACC_LOG_INFO(...) kebacc_log(LOG_INFO, "", __LINE__, __VA_ARGS__)
    #define KEBACC_LOG_DEBUG(...) kebacc_log(LOG_DEBUG, "", __LINE__, __VA_ARGS__)
    #define KEBACC_LOG_ERRNO(ERRNUM) kebacc_log_errno("", __LINE__, ERRNUM)
#endif

/**
 * Global log variables
 */
extern _Atomic int loglevel;
extern bool log_on_tty;

const char *get_loglevel_name(int level);
void set_loglevel(int level);

void kebacc_log_errno(const char *file, int line, int errnum);
void kebacc_log(int level, const char *file, int line, const char *fmt, ...)
    __attribute__ ((format (printf, 4, 5)));

#endif
