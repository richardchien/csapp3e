/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 *
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "memlib.h"
#include "mm.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "ateam",
    /* First member's full name */
    "Harry Bovik",
    /* First member's email address */
    "bovik@cs.cmu.edu",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    "",
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define WSIZE 4
#define DSIZE 8
#define CHUNKSIZE (1 << 12)

#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define PACK(size, alloc) ((size) | (alloc))

#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

#define HDRP(bp) ((void *)(bp)-WSIZE)
#define FTRP(bp) (HDRP(bp) + GET_SIZE(HDRP(bp)) - WSIZE)
#define PREV_FTRP(bp) (HDRP(bp) - WSIZE)

#define NEXT_BLKP(bp) ((void *)(bp) + GET_SIZE(HDRP(bp)))
#define PREV_BLKP(bp) ((void *)(bp)-GET_SIZE(PREV_FTRP(bp)))

static void *heap_listp;

static void *extend_heap(size_t words);
static void *find_fit(size_t blksize);
static void place(void *bp, size_t blksize);
static void *coalesce(void *bp);

/*
 * mm_init - initialize the malloc package.
 */
int mm_init(void) {
    if ((heap_listp = mem_sbrk(4 * WSIZE)) == (void *)-1) {
        return -1;
    }
    PUT(heap_listp, 0); // 起始位置用于对齐的填充字
    PUT(heap_listp + 1 * WSIZE, PACK(DSIZE, 1)); // 序言块头部
    PUT(heap_listp + 2 * WSIZE, PACK(DSIZE, 1)); // 序言块脚部
    PUT(heap_listp + 3 * WSIZE, PACK(0, 1)); // 结尾块头部
    heap_listp += 2 * WSIZE; // 指向序言块头脚中间

    if (extend_heap(CHUNKSIZE / WSIZE) == NULL) {
        return -1;
    }
    return 0;
}

static void *extend_heap(size_t words) {
    void *bp;
    size_t size = (words % 2 == 1) ? (words + 1) * WSIZE : words * WSIZE;
    if ((bp = mem_sbrk(size)) == (void *)-1) {
        return NULL;
    }

    // 此时 bp 指向原先的堆区结尾，bp 的前一个字是结尾块头部
    PUT(HDRP(bp), PACK(size, 0)); // 将原来的结尾块头部作为新的空闲块头部
    PUT(FTRP(bp), PACK(size, 0)); // 由于前面的头部使用的是原本已经分配的空间，因此这里脚部占在堆区倒数第二个字
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); // 新的结尾块

    return coalesce(bp);
}

/*
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size) {
    // int newsize = ALIGN(size + SIZE_T_SIZE);
    // void *p = mem_sbrk(newsize);
    // if (p == (void *)-1)
    //     return NULL;
    // else {
    //     *(size_t *)p = size;
    //     return (void *)((char *)p + SIZE_T_SIZE);
    // }

    if (size == 0) return NULL;

    size_t blksize = ALIGN(size) + DSIZE; // aligned size + size of header and footer
    void *bp;
    if ((bp = find_fit(blksize)) != NULL) {
        place(bp, blksize);
        return bp;
    }

    size_t extendsize = MAX(blksize, CHUNKSIZE);
    if ((bp = extend_heap(extendsize / WSIZE)) == NULL) {
        return NULL;
    }
    place(bp, blksize);
    return bp;
}

static void *find_fit(size_t blksize) {
    size_t currsize;
    for (void *bp = heap_listp; (currsize = GET_SIZE(HDRP(bp))) != 0; bp = NEXT_BLKP(bp)) {
        if (GET_ALLOC(HDRP(bp))) continue;
        if (currsize >= blksize) return bp;
    }
    return NULL;
}

static void place(void *bp, size_t blksize) {
    size_t oldsize = GET_SIZE(HDRP(bp));
    size_t newsize = oldsize - blksize;
    PUT(HDRP(bp), PACK(blksize, 1));
    PUT(FTRP(bp), PACK(blksize, 1));
    PUT(HDRP(NEXT_BLKP(bp)), PACK(newsize, 0));
    PUT(FTRP(NEXT_BLKP(bp)), PACK(newsize, 0));
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *bp) {
    size_t size = GET_SIZE(HDRP(bp));
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    coalesce(bp);
}

static void *coalesce(void *bp) {
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(FTRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));

    if (prev_alloc && next_alloc) {
        return bp;
    }

    if (prev_alloc && !next_alloc) {
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
    } else if (!prev_alloc && next_alloc) {
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        bp = PREV_BLKP(bp);
    } else { // !prev_alloc && !next_alloc
        size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(HDRP(NEXT_BLKP(bp)));
        bp = PREV_BLKP(bp);
    }
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    return bp;
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size) {
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;

    newptr = mm_malloc(size);
    if (newptr == NULL) return NULL;
    copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    if (size < copySize) copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}
