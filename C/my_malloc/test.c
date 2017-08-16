#include "my_malloc.h"

/* These have been provided for convenience */
void print_metadata(metadata_t* block) {
    printf("%p - { next = %p, size = %lu }\n", (void*)block, (void*)block->next, block->size);
}

void print_block_metadata(void* block) {
    print_metadata((metadata_t*)block - 1);
}

/* These have been provided for convenience */
void print_freelist() {
    printf("\n--------FREELIST--------\n");

    extern metadata_t* freelist;
    for(metadata_t* curr = freelist; curr; curr = curr->next) {
        print_metadata(curr);
    }

    char* name = NULL;
    switch(ERRNO) {
        case NO_ERROR:
            name = "NO_ERROR";
            break;
        case OUT_OF_MEMORY:
            name = "OUT_OF_MEMORY";
            break;
        case SINGLE_REQUEST_TOO_LARGE:
            name = "SINGLE_REQUEST_TOO_LARGE";
            break;
        case CANARY_CORRUPTED:
            name = "CANARY_CORRUPTED";
            break;
    }

    printf("ERRNO = %s\n", name);
}

int main() {
    printf("Allocate a 1-byte block\n");
    void* b1 = my_malloc(1);
    print_block_metadata(b1);
    print_freelist();

    printf("Allocate a 2-byte block\n");
    void* b2 = my_malloc(2);
    print_block_metadata(b2);

    print_freelist();

    printf("Allocate a 9-byte block\n");
    void* b3 = my_malloc(9);
    print_block_metadata(b3);

    print_freelist();


    /* Put your own tests here! */

    return 0;
}
