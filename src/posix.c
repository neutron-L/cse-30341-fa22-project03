/* posix.c: POSIX API Implementation */

#include "malloc/counters.h"
#include "malloc/freelist.h"

#include <assert.h>
#include <string.h>

extern Block FreeList;

/**
 * Allocate specified amount memory.
 * @param   size    Amount of bytes to allocate.
 * @return  Pointer to the requested amount of memory.
 **/
void *malloc(size_t size) {
    // Initialize counters
    init_counters();

    // Handle empty size
    if (!size) {
        return NULL;
    }

    // TODO: Search free list for any available block with matching size
    Block *block = free_list_search(ALIGN(size));

    if (block)
    {
        block_split(block, size);
        block_detach(block);
    }
    else
        block = block_allocate(size);

    // Could not find free block or allocate a block, so just return NULL
    if (!block) {
        return NULL;
    }

    // Check if allocated block makes sense
    assert(block->capacity >= block->size);
    assert(block->size     == size);
    assert(block->next     == block);
    assert(block->prev     == block);

    // Update counters
    Counters[MALLOCS]++;
    Counters[REQUESTED] += size;

    // Return data address associated with block
    return block->data;
}

/**
 * Release previously allocated memory.
 * @param   ptr     Pointer to previously allocated memory.
 **/
void free(void *ptr) {
    if (!ptr) {
        return;
    }

    // Update counters
    Counters[FREES]++;

    // TODO: Try to release block, otherwise insert it into the free list
    Block * block = (Block *)ptr - 1;
    block->size = 0;
    free_list_insert(block);
    block_release(FreeList.prev);
}

/**
 * Allocate memory with specified number of elements and with each element set
 * to 0.
 * @param   nmemb   Number of elements.
 * @param   size    Size of each element.
 * @return  Pointer to requested amount of memory.
 **/
void *calloc(size_t nmemb, size_t size) {
    // TODO: Implement calloc
    if (!nmemb || nmemb * size / nmemb != size)
        return NULL;
    Block * block = malloc(nmemb * size);

    if (block)
    {
        memset(block->data, 0, block->capacity);
        Counters[CALLOCS]++;

        return block->data;
    }
    else
        return NULL;
}

/**
 * Reallocate memory with specified size.
 * @param   ptr     Pointer to previously allocated memory.
 * @param   size    Amount of bytes to allocate.
 * @return  Pointer to requested amount of memory.
 **/
void *realloc(void *ptr, size_t size) {
    // TODO: Implement realloc
    if (!ptr)
    {
        Counters[REALLOCS]++;
        return malloc(size);
    }
    else
    {
        Block *block = (Block *)ptr - 1;
        void * new_ptr;
        if (block->capacity >= size)
        {
            block_split(block, size);
            new_ptr = block->data;
        }
        else
        {
            void * old_ptr = ptr;
            size_t old_sz = block->size;
            free(old_ptr);
            new_ptr = malloc(size);

            memmove(new_ptr, old_ptr, old_sz);
        }

        Counters[REALLOCS]++;
        return new_ptr;
    }    
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
