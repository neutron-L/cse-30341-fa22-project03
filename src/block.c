/* block.c: Block Structure */

#include "malloc/block.h"
#include "malloc/counters.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

/**
 * Allocate a new block on the heap using sbrk:
 *
 *  1. Determined aligned amount of memory to allocate.
 *  2. Allocate memory on the heap.
 *  3. Set allocate block properties.
 *
 * @param   size    Number of bytes to allocate.
 * @return  Pointer to data portion of newly allocate block.
 **/
Block *	block_allocate(size_t size) {
    // Allocate block
    intptr_t allocated = sizeof(Block) + ALIGN(size);
    Block *  block     = sbrk(allocated);
    if (block == SBRK_FAILURE) {
        return NULL;
    }

    // Record block information
    block->capacity = ALIGN(size);
    block->size     = size;
    block->prev     = block;
    block->next     = block;

    // Update counters
    Counters[HEAP_SIZE] += allocated;
    Counters[BLOCKS]++;
    Counters[GROWS]++;
    return block;
}

/**
 * Attempt to release memory used by block to heap:
 *
 *  1. If the block is at the end of the heap.
 *  2. The block allocation meets the trim threshold.
 *
 * @param   block   Pointer to block to release.
 * @return  Whether or not the release completed successfully.
 **/
bool	block_release(Block *block) {
    size_t allocated = sizeof(Block) + block->capacity;

    // TODO: Implement block release
    if ((char *)block + allocated == sbrk(0) && block->capacity > TRIM_THRESHOLD)
    {
        block_detach(block);

        sbrk(-allocated);
    
        Counters[BLOCKS]--;
        Counters[SHRINKS]++;
        Counters[HEAP_SIZE] -= allocated;

        return true;
    }
    
    return false;
}

/**
 * Detach specified block from its neighbors.
 *
 * @param   block   Pointer to block to detach.
 * @return  Pointer to detached block.
 **/
Block * block_detach(Block *block) {
    // TODO: Detach block from neighbors by updating previous and next block
    if (block)
    {
        Block * prev = block->prev;
        Block * next = block->next;
        block->prev = block->next = block;
        prev->next = next;
        next->prev = prev;
    }

    return block;
}

/**
 * Attempt to merge source block into destination.
 *
 *  1. Compute end of destination and start of source.
 *
 *  2. If they both match, then merge source into destination by giving the
 *  destination all of the memory allocated to source.
 *
 *  3. Update references from and to destination block appropriately.
 *
 * @param   dst     Destination block we are merging into.
 * @param   src     Source block we are merging from.
 * @return  Whether or not the merge completed successfully.
 **/
bool	block_merge(Block *dst, Block *src) {
    // TODO: Implement block merge
    if ((char*)dst + dst->capacity + sizeof(Block) == (char *)src)
    {
        dst->next = src->next;
        src->next->prev = dst;
        dst->capacity += src->capacity + sizeof(Block);

        Counters[MERGES]++;
        Counters[BLOCKS]--;

        return true;
    }
    
    return false;
}

/**
 * Attempt to split block with the specified size:
 *
 *  1. Check if block capacity is sufficient for requested aligned size and
 *  header Block.
 *
 *  2. Split specified block into two blocks.
 *
 * @param   block   Pointer to block to split into two separate blocks.
 * @param   size    Desired size of the first block after split.
 * @return  Pointer to original block (regardless if it was split or not).
 **/
Block * block_split(Block *block, size_t size) {
    // TODO: Implement block split
    block->size = size;
    size = ALIGN(size);
    if (block->capacity - size > sizeof(Block))
    {
        Block * new_block = (Block *)((char *)block + sizeof(Block) + size);
        new_block->capacity = block->capacity - size - sizeof(Block);
        new_block->size = 0;
        block->capacity = size;

        new_block->next = block->next;
        block->next->prev = new_block;
        
        block->next = new_block;
        new_block->prev = block;

        Counters[SPLITS]++;
        Counters[BLOCKS]++;
    }
    
    return block;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
