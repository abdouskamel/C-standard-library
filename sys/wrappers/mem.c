#include "mem.h"
#include <sys/syscall.h>
#include <stdio.h>

/*
 * Note that this is kernel brk.
 * Return the new break on success,
 * and the current break on failure.
 */
void *brk(void *addr)
{
    syscall_64ret_noerrno(SYS_brk, void *);
    return (ret);
}

/*
 *
 */
void *sbrk(int64_t incr)
{
    void *cur_brk = brk(NULL);
    void *new_brk = brk((char *)cur_brk + incr);

    if (cur_brk == new_brk)
        return (NULL);

    return (cur_brk);
}
