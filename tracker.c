#include <stdio.h>
#include <stdlib.h>
#include "tracker.h"

static Allocation* allocs = NULL;
static size_t count = 0;
static size_t bytes_in_use = 0;
static size_t next_alloc = 1; // incremented id

void* mt_malloc_impl(size_t size, const char* file, int line){
    void* location = malloc(size);
    if (location == NULL){
        printf("MAlloc failed");
        return NULL;
    }

    Allocation* new_allocs = realloc(allocs, (count + 1) * sizeof(Allocation)); // redundancy
    if (new_allocs == NULL){
        free(location);
        printf("Failed to track allocation");
        return NULL;
    }
    
    allocs = new_allocs;
    allocs[count].id = next_alloc;

    allocs[count].ptr = location;
    allocs[count].size = size;
    allocs[count].alloc_file = file;
    allocs[count].alloc_line = line;
    allocs[count].free_file = NULL;
    allocs[count].free_line = 0;
    allocs[count].state = MT_LIVE;

    bytes_in_use += size;
    count++;
    next_alloc++;

    printf("Allocated %zu bytes to %p (total: %zu)\n", size, location, bytes_in_use);
    return location;
}

void mt_free_impl(void* ptr, const char* file, int line){
    if (!ptr){
        return;
    }

    for (size_t i = count; i-- > 0;){
        
        if (allocs[i].ptr == ptr){
            if (allocs[i].state == MT_LIVE){
                // valid free
                bytes_in_use -= allocs[i].size;
                allocs[i].free_file = file;
                allocs[i].free_line = line;
                allocs[i].state = MT_FREED;

                free(ptr);
                return;
            } else {
                // double free
                printf("Yooo this is a double free of %p at %s:%d sort this out", ptr, file, line);
                printf(" this was allocated at %s:%d (id=%zu, size=%zu)",
                     allocs[i].alloc_file, allocs[i].alloc_line, allocs[i].id, allocs[i].size);
                printf(" first freed at %s:%d\n", allocs[i].free_file, allocs[i].free_line);
                return;
            }
        }
    }
    // invalid free
    printf("INVALID FREE of %p at %s:%d (pointer not tracked)\n", ptr, file, line);
}

void mt_print_stats(){
    printf("Total bytes in use: %zu\n", bytes_in_use);
    printf("Total allocations ever: %zu\n", count);
    for (size_t i = 0; i < count; i++){
        const char* state_str = (allocs[i].state == MT_LIVE) ? "LIVE" : "FREED";
        printf("[%s] ID %zu: %p (%zu bytes) alloc at %s:%d", 
               state_str, allocs[i].id, allocs[i].ptr, allocs[i].size, 
               allocs[i].alloc_file, allocs[i].alloc_line);
        if (allocs[i].state == MT_FREED) {
            printf(" - freed at %s:%d", allocs[i].free_file, allocs[i].free_line);
        }
        printf("\n");
    }
}
