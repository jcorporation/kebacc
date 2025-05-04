/*
 SPDX-License-Identifier: GPL-3.0-or-later
 (c) 2025 Juergen Mang <mail@jcgames.de>
 https://github.com/jcorporation/kebacc
*/

/*! \file
 * \brief File handling
 */

#include "compile_time.h"
#include "src/lib/filehandler.h"

#include "src/lib/log.h"
#include "src/lib/mem.h"

#include <errno.h>
#include <string.h>
#include <unistd.h>


/**
 * Writes data to a file, overwrites existing files.
 * @param basepath Base path to write to
 * @param filename Filename to write
 * @param data Data to write
 * @param data_len Data length to write
 * @return true on success else false
 */
bool write_data_to_file(const char *basepath, const char *filename, const char *data, size_t data_len) {
    char tmp_file[1024];
    int len = snprintf(tmp_file, 1024, "%s/%s.XXXXXX", basepath, filename);
    if (len < 0 || len >= 1024) {
        KEBACC_LOG_ERROR("Filepath is to long");
        return false;
    }
    FILE *fp = open_tmp_file(tmp_file);
    if (fp == NULL) {
        return false;
    }
    size_t written = fwrite(data, 1, data_len, fp);
    if (written != data_len) {
        rm_file(tmp_file);
        return false;
    }
    return rename_tmp_file(fp, tmp_file);
}

/**
 * Opens a temporary file for write using mkstemp
 * @param filepath filepath to open, e.g. /tmp/test.XXXXXX
 *                 XXXXXX is replaced with a random string
 * @return FILE pointer
 */
FILE *open_tmp_file(char *filepath) {
    errno = 0;
    int fd = mkstemp(filepath);
    if (fd < 0) {
        KEBACC_LOG_ERROR("Can not open tmp file descriptor \"%s\" for write", filepath);
        KEBACC_LOG_ERRNO(errno);
        return NULL;
    }
    errno = 0;
    FILE *fp = fdopen(fd, "w");
    if (fp == NULL) {
        KEBACC_LOG_ERROR("Can not open tmp file \"%s\" for write", filepath);
        KEBACC_LOG_ERRNO(errno);
    }
    return fp;
}

/**
 * Closes the tmp file and moves it to its destination name
 * This is done by removing the last 7 characters from the tmp_file.
 * See open_tmp_file for corresponding open function.
 * @param fp FILE pointer
 * @param tmp_file tmp file to close and move
 * @param write_rc if false tmp file will be removed
 * @return true on success else false
 */
bool rename_tmp_file(FILE *fp, const char *tmp_file) {
    if (fclose(fp) != 0) {
        KEBACC_LOG_ERROR("Error writing data to file \"%s\"", tmp_file);
        rm_file(tmp_file);
        return false;
    }
    errno = 0;
    //filepath is tmp_file without .XXXXXX suffix
    char *filepath = strdup(tmp_file);
    filepath[(strlen(tmp_file) - 7)] = '\0';
    if (rename(tmp_file, filepath) == -1) {
        KEBACC_LOG_ERROR("Rename file from \"%s\" to \"%s\" failed", tmp_file, filepath);
        KEBACC_LOG_ERRNO(errno);
        rm_file(tmp_file);
        FREE_PTR(filepath);
        return false;
    }
    FREE_PTR(filepath);
    return true;
}

/**
 * Removes a file and logs errors
 * @param filepath filepath to remove
 * @return true on success else false
 */
bool rm_file(const char *filepath) {
    errno = 0;
    if (unlink(filepath) != 0) {
        KEBACC_LOG_ERROR("Error removing file \"%s\"", filepath);
        KEBACC_LOG_ERRNO(errno);
        return false;
    }
    return true;
}
