#ifndef MEM_MALLOC_H
#define MEM_MALLOC_H

#include <stdio.h>
#include <stdint.h>

/*
 * Init our implicit free list heap.
 * Return 0 on success, -1 on failure.
 */
int mem_init(void);

/*
 * Extend the heap by size bytes.
 * On success, return a pointer to the location of the new free block.
 * On failure, return NULL.
 */
void *extend_heap(int64_t size);

/*
 * Try to merge free blocks that surround pp.
 * pp must be a payload pointer, not a block pointer.
 * Return a pointer to the payload of the eventually new block.
 */
void *coalesce(void *pp);

/*
 * Allocate a block of size bytes in the heap, and returns a pointer to it.
 * This function doesn't accept 0 byte blocks.
 * This function returns NULL on failure.
 */
void *malloc(size_t size);

/*
 * Use first-fit to find a suitable free block for asize.
 * Note : asize must include the size of the header and the footer,
 * and must conform to blocks size constraints.
 *
 * Return a pointer to the payload of the suitable block.
 * If no block were found, return NULL.
 */
void *find_fit(size_t asize);

/*
 * Place a block of size asize at the location given by pp.
 * Note that this function doesn't check if bp is a valid location.
 */
void place_block(void *pp, size_t asize);

/*
 * Deallocate a previous allocated block, who's payload is pointed by ptr.
 * If ptr doesn't points to a valid payload, the behavior of the function is undefined.
 * If ptr is NULL, the function does nothing.
 */
void free(void *pp);

#endif
