#ifndef MEM_TYPES_H
#define MEM_TYPES_H

#include <stdint.h>

// Size of a word and a double word
#define WSIZE 4
#define DSIZE 8

// The minimum value with which the heap can be enlarged 
#define CHUNKSIZE (1 << 12)

#define MAX(x, y) ((x) > (y) ? (x) : (y))

// PACK is used to create a block header or footer 
#define PACK(size, alloc) ((size) | (alloc))

// Get and write a 64-bit value to location pointed by p 
#define GET(p) (*(uint64_t *)(p))
#define PUT(p, val) (*(uint64_t *)(p) = (val))

// Header and footer elements accessors 
#define GET_SIZE(bp) (GET(bp) & ~0x0f)
#define GET_ALLOC(bp) (GET(bp) & 0x01)

// Get header and footer from payload ptr 
#define HDRP(pp) ((char *)(pp)-DSIZE)
#define FTRP(pp) ((char *)(pp) + GET_SIZE(HDRP(pp)) - 2 * DSIZE)

// Get next and previous payloads ptr 
#define NEXT_PP(pp) ((char *)(pp) + GET_SIZE(HDRP(pp)))
#define PREV_PP(pp) ((char *)(pp)-GET_SIZE((char *)(pp)-2 * DSIZE))

#endif
