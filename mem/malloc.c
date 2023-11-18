#include "malloc.h"
#include "types.h"

#include <sys/wrappers/mem.h>
#include <sys/wrappers/io.h>

static char *heap_listp;

/*
 * Init our implicit free list heap.
 * Return 0 on success, -1 on failure.
 */
int mem_init(void)
{
    if ((heap_listp = sbrk(DSIZE * 3)) == NULL)
        return (-1);

    // Prologue header
    PUT(heap_listp, PACK(2 * DSIZE, 1));

    // Prologue footer
    heap_listp += DSIZE;
    PUT(heap_listp, PACK(2 * DSIZE, 1));

    // Epilogue header
    PUT(heap_listp + DSIZE, PACK(0, 1));

    // The heap is at first CHUNKSIZE bytes
    if (extend_heap(CHUNKSIZE) == NULL)
        return (-1);

    return (0);
}

/*
 * Extend the heap by size bytes.
 * On success, return a pointer to the location of the new free block.
 * On failure, return NULL.
 */
void *extend_heap(int64_t size)
{
    char *pp;
    if ((pp = sbrk(size)) == NULL)
        return (NULL);

    PUT(HDRP(pp), PACK(size, 0));
    PUT(FTRP(pp), PACK(size, 0));
    PUT(HDRP(NEXT_PP(pp)), PACK(0, 1));

    return (coalesce(pp));
}

/*
 * Try to merge free blocks that surround pp.
 * pp must be a payload pointer, not a block pointer.
 * Return a pointer to the payload of the eventually new block.
 */
void *coalesce(void *pp)
{
    char prev_alloc = GET_ALLOC(FTRP(PREV_PP(pp)));
    char next_alloc = GET_ALLOC(HDRP(NEXT_PP(pp)));
    size_t size = GET_SIZE(HDRP(pp));

    if (prev_alloc && next_alloc)
        return (pp);

    else if (prev_alloc && !next_alloc)
    {
        size += GET_SIZE(HDRP(NEXT_PP(pp)));
        PUT(HDRP(pp), PACK(size, 0));
        PUT(FTRP(pp), PACK(size, 0));
    }

    else if (!prev_alloc && next_alloc)
    {
        size += GET_SIZE(FTRP(PREV_PP(pp)));

        PUT(FTRP(pp), PACK(size, 0));
        pp = PREV_PP(pp);
        PUT(HDRP(pp), PACK(size, 0));
    }

    else
    {
        size += GET_SIZE(FTRP(PREV_PP(pp))) + GET_SIZE(HDRP(NEXT_PP(pp)));
        PUT(FTRP(NEXT_PP(pp)), PACK(size, 0));

        pp = PREV_PP(pp);
        PUT(HDRP(pp), PACK(size, 0));
    }

    return (pp);
}

/*
 * Allocate a block of size bytes in the heap, and returns a pointer to it.
 * This function doesn't accept 0 byte blocks.
 * This function returns NULL on failure.
 */
void *malloc(size_t size)
{
    if (size == 0)
        return (NULL);

    // We add to size header and footer sizes, and we round it up for 16-byte alignment
    size_t asize = 2 * DSIZE * ((size + 2 * DSIZE + (2 * DSIZE - 1)) / (2 * DSIZE));

    char *pp;
    if ((pp = find_fit(asize)) != NULL)
    {
        place_block(pp, asize);
        return (pp);
    }

    // We don't want to extend the heap by a value inferior to CHUNKSIZE
    size_t extendsize = MAX(asize, CHUNKSIZE);
    if ((pp = extend_heap(extendsize)) == NULL)
        return (NULL);

    place_block(pp, extendsize);
    return (pp);
}

/*
 * Use first-fit to find a suitable free block for asize.
 * Note : asize must include the size of the header and the footer,
 * and must conform to blocks size constraints.
 *
 * Return a pointer to the payload of the suitable block.
 * If no block were found, return NULL.
 */
void *find_fit(size_t asize)
{
    char *pp = heap_listp + 2 * DSIZE;
    size_t size = GET_SIZE(HDRP(pp));

    while (size != 0)
    {
        if (!GET_ALLOC(HDRP(pp)) && asize <= size)
            return (pp);

        pp = NEXT_PP(pp);
        size = GET_SIZE(HDRP(pp));
    }

    return (NULL);
}

/*
 * Place a block of size asize at the location given by pp.
 * Note that this function doesn't check if pp is a valid location.
 */
void place_block(void *pp, size_t asize)
{
    size_t size = GET_SIZE(HDRP(pp));

    if (asize < size)
    {
        PUT(FTRP(pp), PACK(size - asize, 0));
        PUT((char *)pp + asize - DSIZE, PACK(size - asize, 0));
    }

    PUT(HDRP(pp), PACK(asize, 1));
    PUT(FTRP(pp), PACK(asize, 1));
}

/*
 * Deallocate a previous allocated block, who's payload is pointed by ptr.
 * If ptr doesn't points to a valid payload, the behavior of the function is undefined.
 * If ptr is NULL, the function does nothing.
 */
void free(void *pp)
{
    if (pp == NULL)
        return;

    size_t size = GET_SIZE(HDRP(pp));
    PUT(HDRP(pp), PACK(size, 0));
    PUT(HDRP(pp), PACK(size, 0));

    coalesce(pp);
}
