#include "memlib.h"
#include <stdlib.h>
#include <errno.h>

#define MAX_HEAP 0xffffffff

static char *mem_heap;     /* First byte of the heap */
static char *mem_brk;      /* First byte not in the heap */
static char *mem_heap_end; /* Points to the end of the heap + 1 */

void mem_init(void)
{
    mem_heap = malloc(MAX_HEAP);
    mem_brk = mem_heap;
    mem_heap_end = mem_heap + MAX_HEAP;
}

void *mem_sbrk(int incr)
{
    char *old_brk = mem_brk;
    if (incr < 0 || mem_brk + incr > mem_heap_end)
    {
        errno = ENOMEM;
        return (NULL);
    }

    mem_brk += incr;
    return (old_brk);
}
