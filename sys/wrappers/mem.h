#ifndef SYS_WRAPPERS_MEM_H
#define SYS_WRAPPERS_MEM_H

#include <stdint.h>

/*
 * This is kernel brk.
 * Return the new break on success and the current break on failure.
 */
void *brk(void *addr);

/*
 *
 */
void *sbrk(int64_t inc);

#endif
