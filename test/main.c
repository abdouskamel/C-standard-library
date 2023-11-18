#include <stdio.h>
#include <stdlib.h>
#include "memlib.h"

#define WSIZE 4
#define DSIZE 8
#define CHUNKSIZE (1 << 12)

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define PACK(size, alloc) ((size) | (alloc))

#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

#define GET_SIZE(p) (GET(p) & ~0x07)
#define GET_ALLOC(p) (GET(p) & 0x01)

#define HDRP(bp) ((char *)(bp)-WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)))
#define PREV_BLKP(bp) ((char *)(bp)-GET_SIZE(((char *)(bp)-DSIZE)))

static char *heap_listp = NULL;

int mm_init(void);
static void *extend_heap(size_t words);

void *mm_malloc(size_t size);
static void *find_fit(size_t asize);
static void place(void *bp, size_t asize);

void mm_free(void *ptr);
static void *coalesce(void *bp);

int main()
{
    return (0);
}

int mm_init(void)
{
    if ((heap_listp = mem_sbrk(4 * WSIZE)) == NULL)
        return (-1);

    PUT(heap_listp, 0);
    PUT(heap_listp + WSIZE, PACK(DSIZE, 1));
    PUT(heap_listp + 2 * WSIZE, PACK(DSIZE, 1));
    PUT(heap_listp + 3 * WSIZE, PACK(0, 1));

    heap_listp += 2 * WSIZE;

    if (extend_heap(CHUNKSIZE / WSIZE) == NULL)
        return (-1);

    return (0);
}

static void *extend_heap(size_t words)
{
    size_t size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;

    char *bp = mem_sbrk(size);
    if (bp == NULL)
        return (NULL);

    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));

    /* Coalesce if the previous block was free */
    return (coalesce(bp));
}

void *mm_malloc(size_t size)
{
    if (size == 0)
        return (NULL);

    size_t asize;
    size_t extendsize;
    char *bp;

    if (size <= DSIZE)
        asize = 2 * DSIZE;

    else
        asize = DSIZE * ((size + DSIZE + (DSIZE - 1)) / DSIZE);

    if ((bp = find_fit(asize)) != NULL)
    {
        place(bp, asize);
        return (bp);
    }

    extendsize = MAX(asize, CHUNKSIZE);
    if ((bp = extend_heap(extendsize / WSIZE)) == NULL)
        return (NULL);

    place(bp, asize);
    return (bp);
}

static void *find_fit(size_t asize)
{
    char *bp = heap_listp;
    size_t size = GET_SIZE(HDRP(bp));

    while (size != 0)
    {
        if (size >= asize)
            return (bp);

        bp = NEXT_BLKP(bp);
        size = GET_SIZE(HDRP(bp));
    }

    return (NULL);
}

static void place(void *bp, size_t asize)
{
    size_t bsize = GET_SIZE(HDRP(bp));

    if (asize < bsize)
    {
        PUT(FTRP(bp), PACK(bsize - asize, 0));
        PUT((char *)bp + asize + WSIZE, PACK(bsize - asize, 0));
    }

    PUT(HDRP(bp), PACK(asize, 1));
    PUT(FTRP(bp), PACK(asize, 1));
}

void mm_free(void *ptr)
{
    size_t size = GET_SIZE(HDRP(ptr));

    PUT(HDRP(ptr), PACK(size, 0));
    PUT(FTRP(ptr), PACK(size, 0));
    coalesce(ptr);
}

static void *coalesce(void *bp)
{
    int prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    int next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));

    if (prev_alloc && next_alloc)
        return (bp);

    else if (prev_alloc && !next_alloc)
    {
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
    }

    else if (!prev_alloc && next_alloc)
    {
        size += GET_SIZE(FTRP(PREV_BLKP(bp)));
        PUT(FTRP(bp), PACK(size, 0));

        bp = PREV_BLKP(bp);
        PUT(HDRP(bp), PACK(size, 0));
    }

    else
    {
        size += GET_SIZE(HDRP(NEXT_BLKP(bp))) + GET_SIZE(FTRP(PREV_BLKP(bp)));

        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));

        bp = PREV_BLKP(bp);
    }

    return (bp);
}
