/**_**_*****_*_*********************
 *   Student ID: har131              *
 *   Student name: Harris Roberts   *
 ***********************************/

// C libraries for C things
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

// This file provides the memory management functions we need for our
// implementation
#include "memlib.h"
// This file just contains some function declarations
#include "mm.h"

#define UNSCALED_POINTER_ADD(p, x) ((void *)((char *)(p) + (x)))
#define UNSCALED_POINTER_SUB(p, x) ((void *)((char *)(p) - (x)))

typedef struct _BlockInfo {
  long int size;       // Size of block
  struct _Block *prev; // Explicit pointer to previous Block
} BlockInfo_t;

typedef struct _FreeBlockInfo {
  struct _Block *nextFree; // Explicit pointer to next free Block (stage 3)
  struct _Block *prevFree; // Explicit pointer to previous free Block (stage 3)
} FreeBlockInfo_t;

typedef struct _Block {
  BlockInfo_t info;         // Composing both infos into a single struct
  FreeBlockInfo_t freeNode; //  Think: What does this mean in terms of memory?
} Block_t;

typedef struct {
  Block_t *malloc_list_tail; // Use keep track of the tail node
  Block_t *free_list_head;   // Pointer to the first FreeBlockInfo_t in the free
                             // list, use in stage 3.
} malloc_info_t;

/* Variable to keep malloc information tidy */
static malloc_info_t malloc_info = {
    .free_list_head = NULL,
    .malloc_list_tail = NULL,
};

#define ALIGNMENT (sizeof(FreeBlockInfo_t))

// Declarations
Block_t *first_block();
Block_t *next_block(Block_t *block);
void *request_more_space(size_t request_size);
size_t heap_size();
void insert_free_node(Block_t *block);
void remove_free_node(Block_t *block);

// find first block
Block_t *first_block() {
  // make sure there is a heap
  if (heap_size() == 0) {
    return NULL;
  }

  // return lowest address
  return (Block_t *)mem_heap_lo();
}

// find next block
Block_t *next_block(Block_t *block) {
  // make sure block exists
  if (!block) {
    return NULL;
  }

  // find next block and make sure its in the heap
  Block_t *next = (Block_t *)UNSCALED_POINTER_ADD(
      block, labs(block->info.size) + sizeof(BlockInfo_t));

  if (next >= (Block_t *)UNSCALED_POINTER_ADD(first_block(), heap_size())) {
    return NULL;
  }

  return next;
}

// request more space on the heap
void *request_more_space(size_t request_size) {
  // request size
  void *ret = mem_sbrk(request_size);

  // make sure it returns something
  if (ret == (void *)-1) {
    printf("ERROR: failed to request_more_space\n");
    exit(0);
  }

  return ret;
}

// return the size of the heap
size_t heap_size() { return mem_heapsize(); }

// search all allocations for a free space
Block_t *search_list(size_t request_size) {
  // ptr_free_block will point to the beginning of the memory heap!
  Block_t *ptr_free_block = first_block();
  long int check_size = -request_size;

  // check if the first block exists, is free and is at least big enough, then
  // returns the block found
  while (ptr_free_block != NULL) {
    if (ptr_free_block->info.size < 0 &&
        ptr_free_block->info.size <= check_size) {
        return ptr_free_block;
    }
    ptr_free_block = next_block(ptr_free_block);
  }
  return NULL;
}

// split a block
void split(Block_t *block, size_t size) {
  // get the original size of the block and calculate the remaining size after
  // split
  size_t original_size = labs(block->info.size);
  size_t remaining_size = original_size - size - sizeof(BlockInfo_t);

  // check if the remaining size is big enought to be used, initializes the new
  // block
  if (remaining_size >= sizeof(BlockInfo_t)) {
    Block_t *new_block =
        UNSCALED_POINTER_ADD(block, size + sizeof(BlockInfo_t));

    // initialize sizes and prev
    new_block->info.size = -remaining_size;
    block->info.size = size;
    new_block->info.prev = block;

    Block_t *next = next_block(new_block);
    if (next) {
      next->info.prev = new_block;
    }

    // updates malloc tail
    if (malloc_info.malloc_list_tail == block) {
      malloc_info.malloc_list_tail = new_block;
    }

    // adds the block to free list
    insert_free_node(new_block);
  }
}

// coalesce adjacent blocks
void coalesce(Block_t *block) {
  // get the size of the block, the next block, the previous block, and the
  // next-next block
  size_t size = block->info.size;
  Block_t *nxt_block = next_block(block);
  Block_t *prev_block = block->info.prev;
  Block_t *next_next_block = next_block(nxt_block);

  // check that the previous block exists and is free
  if (prev_block && prev_block->info.size < 0) {
    // remove previous block from free list
    remove_free_node(prev_block);

    // update the size, prev, and malloc list tail
    size -= (labs(prev_block->info.size) + sizeof(BlockInfo_t));
    prev_block->info.size = size;
    if (nxt_block) {
      nxt_block->info.prev = prev_block;
    }
    if (malloc_info.malloc_list_tail == block) {
      malloc_info.malloc_list_tail = prev_block;
    }

    // set block to new initialized block
    block = prev_block;
  }

  // check that next block exists and is free
  if (nxt_block && nxt_block->info.size < 0) {
    // remove next block from free list
    remove_free_node(nxt_block);

    // update size, prev, and malloc list tail
    size -= (labs(nxt_block->info.size) + sizeof(BlockInfo_t));
    block->info.size = size;
    if (next_next_block) {
      next_next_block->info.prev = block;
    }
    if (malloc_info.malloc_list_tail == nxt_block) {
      malloc_info.malloc_list_tail = block;
    }
  }

  // add the block to the free list
  insert_free_node(block);
}

// insert free node in free list
void insert_free_node(Block_t *block) {
  // update block next and prev free
  block->freeNode.nextFree = malloc_info.free_list_head;
  block->freeNode.prevFree = NULL;

  // update free list head prev free if the head is not null
  if (malloc_info.free_list_head != NULL) {
    malloc_info.free_list_head->freeNode.prevFree = block;
  }

  // update free list head
  malloc_info.free_list_head = block;
}

// remove a node from in free list
void remove_free_node(Block_t *block) {
  // if the block's previous free is not null, update the previous free's next
  // free  in order to bypass current block or change the free list head to be
  // next free
  if (block->freeNode.prevFree != NULL) {
    block->freeNode.prevFree->freeNode.nextFree = block->freeNode.nextFree;
  } else {
    malloc_info.free_list_head = block->freeNode.nextFree;
  }

  // if next free is not null, change next free's previous free in order to
  // bypass current node
  if (block->freeNode.nextFree != NULL) {
    block->freeNode.nextFree->freeNode.prevFree = block->freeNode.prevFree;
  }

  // get rid or current nodes metadata
  block->freeNode.nextFree = NULL;
  block->freeNode.prevFree = NULL;
}

// search free list for a block of requested size
Block_t *search_free_list(size_t request_size) {
  // get free list head
  Block_t *ptr_free_block = malloc_info.free_list_head;
  long int check_size = -request_size;

  // search for a free node that is large enought, and return
  while (ptr_free_block != NULL) {
    if (ptr_free_block->info.size <= check_size) {
      return ptr_free_block;
    }
    ptr_free_block = ptr_free_block->freeNode.nextFree;
  }
  return NULL;
}

// initialize lists
int mm_init() {
  malloc_info.free_list_head = NULL;
  malloc_info.malloc_list_tail = NULL;

  return 0;
}

// allocate blocks in the heap
void *mm_malloc(size_t size) {
  Block_t *ptr_free_block = NULL;
  long int request_size;

  if (size == 0) {
    return NULL;
  }

  // Determine the amount of memory we want to allocate
  request_size = size;

  // Round up for correct alignment
  request_size = ALIGNMENT * ((request_size + ALIGNMENT - 1) / ALIGNMENT);

  // search the free list
  ptr_free_block = search_free_list(request_size + sizeof(BlockInfo_t));

  // if there is not a free block large enough, request more space
  if (!ptr_free_block) {
    ptr_free_block =
        (Block_t *)request_more_space(request_size + sizeof(BlockInfo_t));

    // intialize the size and prev
    ptr_free_block->info.size = request_size;
    ptr_free_block->info.prev = malloc_info.malloc_list_tail;

    // update malloc list tail
    malloc_info.malloc_list_tail = ptr_free_block;
  } else {
    // Given a block from free list search, split the block if possible
    size_t block_size = labs(ptr_free_block->info.size);
    if (block_size >= request_size + sizeof(BlockInfo_t)) {
      split(ptr_free_block, request_size);
    }

    // update the size to mark as allocated
    ptr_free_block->info.size = labs(ptr_free_block->info.size);

    // remove the node from the free block
    remove_free_node(ptr_free_block);
  }
  // return the address of the block, plus meta data
  return (void *)UNSCALED_POINTER_ADD(ptr_free_block, sizeof(BlockInfo_t));
}

// free blocks in the heap
void mm_free(void *ptr) {
  // check if the ptr exists
  if (ptr == NULL) {
    return;
  }

  // subtrace metadata from the ptr
  Block_t *block = (Block_t *)UNSCALED_POINTER_SUB(ptr, sizeof(BlockInfo_t));

  // update size to show free
  block->info.size = -labs(block->info.size);

  // call coalese
  coalesce(block);
}

/* Print the heap by iterating through it as an implicit free list. */
void examine_heap() {
  /* print to stderr so output isn't buffered and not output if we crash */
  Block_t *curr = (Block_t *)first_block();
  Block_t *end = (Block_t *)UNSCALED_POINTER_ADD(first_block(), heap_size());
  fprintf(stderr, "====================================================\n");
  fprintf(stderr, "heap size:\t0x%lx\n", heap_size());
  fprintf(stderr, "heap start:\t%p\n", curr);
  fprintf(stderr, "heap end:\t%p\n", end);

  fprintf(stderr, "free_list_head: %p\n", (void *)malloc_info.free_list_head);

  fprintf(stderr, "malloc_list_tail: %p\n",
          (void *)malloc_info.malloc_list_tail);

  while (curr && curr < end) {
    /* print out common block attributes */
    fprintf(stderr, "%p: %ld\t", (void *)curr, curr->info.size);

    /* and allocated/free specific data */
    if (curr->info.size > 0) {
      fprintf(stderr, "ALLOCATED\tprev: %p\n", (void *)curr->info.prev);
    } else {
      fprintf(stderr, "FREE\tnextFree: %p, prevFree: %p, prev: %p\n",
              (void *)curr->freeNode.nextFree, (void *)curr->freeNode.prevFree,
              (void *)curr->info.prev);
    }

    curr = next_block(curr);
  }
  fprintf(stderr, "END OF HEAP\n\n");

  curr = malloc_info.free_list_head;
  fprintf(stderr, "Head ");
  while (curr) {
    fprintf(stderr, "-> %p ", curr);
    curr = curr->freeNode.nextFree;
  }
  fprintf(stderr, "\n");
}

/* Checks the heap data structure for consistency. */
int check_heap() {

  Block_t *curr = (Block_t *)first_block();
  Block_t *end = (Block_t *)UNSCALED_POINTER_ADD(first_block(), heap_size());
  Block_t *last = NULL;
  long int free_count = 0;

  while (curr && curr < end) {
    if (curr->info.prev != last) {
      fprintf(stderr, "check_heap: Error: previous link not correct.\n");
      examine_heap();
    }

    if (curr->info.size <= 0) {
      // Free
      free_count++;
    }

    last = curr;
    curr = next_block(curr);
  }

  curr = malloc_info.free_list_head;
  last = NULL;
  while (curr) {
    if (curr == last) {
      fprintf(stderr, "check_heap: Error: free list is circular.\n");
      examine_heap();
    }
    last = curr;
    curr = curr->freeNode.nextFree;
    if (free_count == 0) {
      fprintf(stderr,
              "check_heap: Error: free list has more items than expected.\n");
      examine_heap();
    }
    free_count--;
  }

  return 0;
}
