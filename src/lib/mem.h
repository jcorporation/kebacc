/*
 SPDX-License-Identifier: GPL-3.0-or-later
 (c) 2025 Juergen Mang <mail@jcgames.de>
 https://github.com/jcorporation/kebacc
*/

#ifndef KEBACC_MEM_H
#define KEBACC_MEM_H

#undef NDEBUG
#include <assert.h>
#include <stdlib.h>

/**
 * Mallocs and asserts if it fails
 * @param size bytes to malloc
 * @return malloced pointer
 */
__attribute__((malloc))
static inline void *malloc_assert(size_t size) {
    void *p = malloc(size);
    assert(p);
    return p;
}

/**
 * Macro to free the pointer if it is not NULL
 * and set it to NULL.
 * @param PTR pointer to free
 */
#define FREE_PTR(PTR) do { \
    if ((PTR) != NULL) \
        free(PTR); \
    (PTR) = NULL; \
} while (0)

#endif
