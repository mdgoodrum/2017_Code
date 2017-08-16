/*
 * CS 2110 Spring 2017
 * Author: Michael Goodrum
 */

/* we need this for uintptr_t */
#include <stdint.h>
/* we need this for memcpy/memset */
#include <string.h>
/* we need this for my_sbrk */
#include "my_sbrk.h"
/* we need this for the metadata_t struct and my_malloc_err enum definitions */
#include "my_malloc.h"

#include <stdlib.h>

/* You *MUST* use this macro when calling my_sbrk to allocate the
 * appropriate size. Failure to do so may result in an incorrect
 * grading!
 */
#define SBRK_SIZE 2048

/* All sizes will be in multiples of the word size defined below */
#define WORD_SIZE 8

/* This is the size of the metadata struct and canary footer in words */
#define TOTAL_METADATA_SIZE ((sizeof(metadata_t) + sizeof(long)) / WORD_SIZE)

/* This is the minimum size of a block in words, where it can
 * store the metadata, canary footer, and at least 1 word of data
 */
#define MIN_BLOCK_SIZE (TOTAL_METADATA_SIZE + 1)

/* If you want to use debugging printouts, it is HIGHLY recommended
 * to use this macro or something similar. If you produce output from
 * your code then you may receive a 20 point deduction. You have been
 * warned.
 */
#ifdef DEBUG
#define DEBUG_PRINT(x) printf(x)
#else
#define DEBUG_PRINT(x)
#endif

/* Our freelist structure - this is where the current freelist of
 * blocks will be maintained. failure to maintain the list inside
 * of this structure will result in no credit, as the grader will
 * expect it to be maintained here.
 * DO NOT CHANGE the way this structure is declared
 * or it will break the autograder.
 */
metadata_t* freelist;

#define BLOCK_NO_DATA_SIZE ( sizeof(long) + sizeof(metadata_t) )

#define round_to_w(i) ((((i)%WORD_SIZE == 0) ? (i) : (i) + (WORD_SIZE - (i)%WORD_SIZE)) / WORD_SIZE)

#define round_to_b(i) (round_to_w(i) + TOTAL_METADATA_SIZE)

#define SECOND_CANARY(block) (*(unsigned long *)((char*)(block) + ((block)->size * WORD_SIZE) - sizeof(unsigned long)))



/* CANARY CALCULATOR
 *
 * Finds the canary value of the passed in block. 
 * If the selected block is null, it return 0.
 */
unsigned long calc_canary(metadata_t* block) {
    	return (uintptr_t)block ^ block->size;
}

// check if two blocks are adjacent in memory
char block_is_adjacent( metadata_t* block_a, metadata_t* block_b ) {
 	 return ((char*)block_a) + block_a->size * WORD_SIZE == (char*)block_b;
}

// merge blocks of adjacent memory
void merge(metadata_t* block_a, metadata_t* block_b) {
	block_a->next = block_b->next;
	block_a->size += block_b->size;
}

// split block and return pointer to second block
void* split( metadata_t* block, size_t request_size ) {
	if (block && request_size) {
		block->size -= request_size;
		metadata_t* new_block = (metadata_t*)((char*)block + (block->size * WORD_SIZE));
		new_block->next = NULL;
		new_block->size = request_size;
		return new_block;
	}
	return NULL;
			
}

int canary_corrupted(metadata_t* block) {
	if (block->canary != calc_canary(block) || (block->canary != SECOND_CANARY(block))) {
		return 1;
	} else {
		return 0;
	}
}
	

// get the end of the freelist
metadata_t* freelist_end() {
  	if (freelist) {
		metadata_t* ptr = freelist;
    		while (ptr->next) {
      			ptr = ptr->next;
    		}
		return ptr;
  	}
  	return NULL;
}


metadata_t* best_fit(size_t request_size) {
	metadata_t* current_best = NULL;
	for (metadata_t* ptr = freelist; ptr; ptr = ptr->next) {
		if (ptr->size == request_size) {
			current_best = ptr;
			return current_best;
		}
		if ((ptr->size > request_size) && (current_best == NULL)) {
			current_best = ptr;
		}
		if ((current_best != NULL) && (current_best->size < (MIN_BLOCK_SIZE + request_size)) && (ptr->size < current_best->size) && (ptr->size > request_size)) {
			current_best = ptr;
		}
		if ((current_best != NULL) && (current_best->size >= (MIN_BLOCK_SIZE + request_size)) && (ptr->size >= (MIN_BLOCK_SIZE + request_size)) && (ptr->size < current_best->size)) {
			current_best = ptr;
		}
		if ((current_best != NULL) && (current_best->size < (MIN_BLOCK_SIZE + request_size)) && (ptr->size >= (MIN_BLOCK_SIZE + request_size))) {
			current_best = ptr;
		}
	}
	if (current_best == NULL) {
		metadata_t* new_ptr = my_sbrk(SBRK_SIZE);
		if (!new_ptr) {
			ERRNO = OUT_OF_MEMORY;
			return NULL;
		}

		new_ptr->next = NULL;
		new_ptr->size = (SBRK_SIZE/WORD_SIZE);
		metadata_t* end_of_freelist = freelist_end();
		

		if (!end_of_freelist) {
			freelist = new_ptr;
			return new_ptr;
		}
		if (block_is_adjacent(end_of_freelist, new_ptr)) {
			merge(end_of_freelist, new_ptr);
		} else {
			end_of_freelist->next = new_ptr;
		}
		return new_ptr;
	}
	return current_best;	
}



/* MALLOC
 * See my_malloc.h for documentation
 */
void* my_malloc(size_t size) {
	ERRNO = NO_ERROR;
	if (size > 0) {
        	const size_t block_size = round_to_b(size);
        	if ((block_size * WORD_SIZE) > SBRK_SIZE ) {
        		ERRNO = SINGLE_REQUEST_TOO_LARGE;
        	} else {
      			metadata_t* best_fit_block = best_fit(block_size);
			if (best_fit_block) {
				if (best_fit_block->size < block_size + MIN_BLOCK_SIZE) {
					if (best_fit_block == freelist) {
						freelist = freelist->next;
					} else {
						for (metadata_t* ptr = freelist; ptr; ptr = ptr->next) {
							if (ptr->next == best_fit_block) {
								ptr->next = best_fit_block->next;
							}
						}
					}
					best_fit_block->canary = calc_canary(best_fit_block);
					SECOND_CANARY(best_fit_block) = best_fit_block->canary;
					return best_fit_block + 1;
				} else {
					metadata_t* split_part = split(best_fit_block, block_size);
					split_part->canary = calc_canary(split_part);
					SECOND_CANARY(split_part) = split_part->canary;
        				return split_part + 1;
      				}
			}
			ERRNO = OUT_OF_MEMORY;
		}
	}
	return NULL;
}

/* REALLOC
 * See my_malloc.h for documentation
 */
void* my_realloc(void* ptr, size_t size) {
	if(ptr == NULL) {
        	return my_malloc(size);
    	}
	if(size == 0) {
		my_free(ptr);
		return NULL;
	}
    	void *returnVal = my_malloc(size);
    	if(returnVal == NULL){
        	return NULL;
    	}
    	memcpy(returnVal, ptr, size);
    	my_free(ptr);

    	return returnVal;
}

/* CALLOC
 * See my_malloc.h for documentation
 */
void* my_calloc(size_t nmemb, size_t size) {
	const size_t dataSize = nmemb * size; 
  	void* dataPointer = my_malloc(dataSize);
	if(dataPointer) {
		memset(dataPointer, 0, dataSize);
	}
  	return dataPointer;
}

/* FREE
 * See my_malloc.h for documentation
 */
void my_free(void* ptr) {
 	ERRNO = NO_ERROR;
  	metadata_t *block = (metadata_t*)ptr - 1;
  	if (!canary_corrupted(block)) {
    		metadata_t *end = freelist_end();
    		if ( freelist == NULL ) { // if no list
      			block->next = NULL;
      			freelist = block;
    		} else if ((uintptr_t)block < (uintptr_t)freelist) {
			if (block_is_adjacent(block, freelist)) {
				merge(block, freelist);
			} else {
      				block->next = freelist;
      				freelist = block;
			}
    		} else if ((uintptr_t)end < (uintptr_t)block) {
			if (block_is_adjacent(end, block)) {
				merge(end, block);
			} else {
      				end->next = block;
      				block->next = NULL;
			}
    		} else if ((uintptr_t)freelist < (uintptr_t)block) {
      			metadata_t *curr_block = freelist->next;
      			metadata_t *prev_block = freelist;
      			while ((uintptr_t)curr_block < (uintptr_t)block) {
        			prev_block = curr_block;
        			curr_block = curr_block->next;
      			}
      			block->next = curr_block;
      			prev_block->next = block;
    		}
    		block = freelist;
    		while (block && block->next) {
      			if (block_is_adjacent(block, block->next)) {
        			merge(block, block->next);
      			} else {
        			block = block->next;
      			}
    		}
  	} else {
   		ERRNO = CANARY_CORRUPTED;
  	}
}
