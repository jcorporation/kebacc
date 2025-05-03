/*
 SPDX-License-Identifier: GPL-3.0-or-later
 (c) 2018-2025 Juergen Mang <mail@jcgames.de>
 https://github.com/jcorporation/kebacc
*/

/*! \file
 * \brief Log implementation
 */

#include "compile_time.h"
#include "src/log.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/**
 * Global variables
 */

/**
 * Loglevel
 */
_Atomic int loglevel;

/**
 * Log goes to tty?
 */
bool log_on_tty;

/**
 * Maps loglevels to names
 */
static const char *loglevel_names[8] = {
    [LOG_EMERG] = "EMERG",
    [LOG_ALERT] = "ALERT",
    [LOG_CRIT] = "CRITICAL",
    [LOG_ERR] = "ERROR",
    [LOG_WARNING] = "WARN",
    [LOG_NOTICE] = "NOTICE",
    [LOG_INFO] = "INFO",
    [LOG_DEBUG] = "DEBUG"
};

/**
 * Maps loglevels to terminal colors
 */
static const char *loglevel_colors[8] = {
    [LOG_EMERG] = "\033[0;31m",
    [LOG_ALERT] = "\033[0;31m",
    [LOG_CRIT] = "\033[0;31m",
    [LOG_ERR] = "\033[0;31m",
    [LOG_WARNING] = "\033[0;33m",
    [LOG_NOTICE] = "",
    [LOG_INFO] = "",
    [LOG_DEBUG] = "\033[0;32m"
};

/**
 * Returns the name of the loglevel
 * @param level 
 * @return the loglevel name or empty string if loglevel is invalid
 */
const char *get_loglevel_name(int level) {
    if (level > LOGLEVEL_MAX ||
        level < LOGLEVEL_MIN)
    {
        return "";
    }
    return loglevel_names[level];
}

/**
 * Sets the loglevel
 * @param level loglevel to set
 */
void set_loglevel(int level) {
    if (level > LOGLEVEL_MAX) {
        level = 7;
    }
    else if (level < LOGLEVEL_MIN) {
        level = 0;
    }
    KEBACC_LOG_NOTICE("Setting loglevel to %s", loglevel_names[level]);
    loglevel = level;
}

/**
 * Logs the errno string
 * This function should be called by the suitable macro
 * @param file filename for debug logging
 * @param line linenumber for debug logging
 * @param partition mpd partition
 * @param errnum errno
 */
void kebacc_log_errno(const char *file, int line, int errnum) {
    if (errnum == 0) {
        //do not log success
        return;
    }
    char err_text[256];
    int rc = strerror_r(errnum, err_text, 256);
    const char *err_str = rc == 0
        ? err_text
        : "Unknown error";
    kebacc_log(LOG_ERR, file, line, "%s", err_str);
}

/**
 * Logs the errno string
 * This function should be called by the suitable macro
 * @param level loglevel of the message
 * @param file filename for debug logging
 * @param line linenumber for debug logging
 * @param fmt format string to print
 * @param ... arguments for the format string
 */
void kebacc_log(int level, const char *file, int line, const char *fmt, ...) {
    if (level > loglevel) {
        return;
    }

    if (log_on_tty == true) {
        printf("%s", loglevel_colors[level]);
        time_t now = time(NULL);
        struct tm timeinfo;
        if (localtime_r(&now, &timeinfo) != NULL) {
            printf("%02d:%02d:%02d ", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
        }
    }
    printf("%-8s ", loglevel_names[level]);
    #ifdef KEBACC_DEBUG
        printf("%s:%i: ", file, line);
    #else
        (void)file;
        (void)line;
    #endif

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    if (log_on_tty == true) {
        printf("%s", "\033[0m\n");
    }
    else {
        printf("%s", "\n");
    }
}
