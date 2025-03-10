/* freelist.c: Free List Implementation
 *
 * The FreeList is an unordered doubly-linked circular list containing all the
 * available memory allocations (memory that has been previous allocated and
 * can be re-used).
 **/

#include "malloc/counters.h"
#include "malloc/freelist.h"

/* Global Variables */

Block FreeList = {-1, -1, &FreeList, &FreeList};

/* Functions */

/**
 * Search for an existing block in free list with at least the specified size
 * using the first fit algorithm.
 * @param   size    Amount of memory required.
 * @return  Pointer to existing block (otherwise NULL if none are available).
 **/
Block *free_list_search_ff(size_t size)
{
    // TODO: Implement first fit algorithm
    Block *cur = FreeList.next;

    while (cur != &FreeList && cur->capacity < size)
        cur = cur->next;

    return cur == &FreeList ? NULL : cur;
}

/**
 * Search for an existing block in free list with at least the specified size
 * using the best fit algorithm.
 * @param   size    Amount of memory required.
 * @return  Pointer to existing block (otherwise NULL if none are available).
 **/
Block *free_list_search_bf(size_t size)
{
    // TODO: Implement best fit algorithm
    Block *cur = FreeList.next;
    Block *target = NULL;

    while (cur != &FreeList)
    {
        if (cur->capacity >= size)
        {
            if (target == NULL || cur->capacity < target->capacity)
                target = cur;
        }
        cur = cur->next;
    }

    return target;
}

/**
 * Search for an existing block in free list with at least the specified size
 * using the worst fit algorithm.
 * @param   size    Amount of memory required.
 * @return  Pointer to existing block (otherwise NULL if none are available).
 **/
Block *free_list_search_wf(size_t size)
{
    // TODO: Implement worst fit algorithm
    Block *cur = FreeList.next;
    Block *target = NULL;

    while (cur != &FreeList)
    {
        if (cur->capacity >= size)
        {
            if (target == NULL || cur->capacity > target->capacity)
                target = cur;
        }
        cur = cur->next;
    }

    return target;
}

/**
 * Search for an existing block in free list with at least the specified size.
 *
 * Note, this is a wrapper function that calls one of the three algorithms
 * above based on the compile-time setting.
 *
 * @param   size    Amount of memory required.
 * @return  Pointer to existing block (otherwise NULL if none are available).
 **/
Block *free_list_search(size_t size)
{
    Block *block = NULL;
#if defined FIT && FIT == 0
    block = free_list_search_ff(size);
#elif defined FIT && FIT == 1
    block = free_list_search_wf(size);
#elif defined FIT && FIT == 2
    block = free_list_search_bf(size);
#endif

    if (block)
    {
        Counters[REUSES]++;
    }
    return block;
}

/**
 * Insert specified block into free list.
 *
 * Scan the free list for the first block whose address is after the given
 * input block and then insert the block into the free list in a manner that
 * ensures that the blocks in the free list are stored in ascending address
 * values (ie. the first block's address is less than the second block's
 * address, etc.).
 *
 * After inserting the block into the free list, attempt to merge block with
 * adjacent blocks.
 *
 * @param   block   Pointer to block to insert into free list.
 **/
void free_list_insert(Block *block)
{
    // TODO: Implement free list insertion
    Block *cur = FreeList.next;

    while (cur != &FreeList && cur < block)
        cur = cur->next;
    
    Block * prev = cur->prev;
    block->prev = prev;
    prev->next = block;

    block->next = cur;
    cur->prev = block;

    block_merge(block, block->next);
    block_merge(block->prev, block);
}

/**
 * Return length of free list.
 * @return  Length of the free list.
 **/
size_t free_list_length()
{
    // TODO: Implement free list length
    Block *cur = FreeList.next;
    size_t len = 0;

    while (cur != &FreeList)
    {
        ++len;
        cur = cur->next;
    }

    return len;
}

void  print_free_list()
{
    Block *cur = FreeList.next;

    while (cur != &FreeList)
    {
        printf("%p: %ld ", cur + sizeof(Block), cur->capacity);
        cur = cur->next;
    }
    printf("\n");
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
