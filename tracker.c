#include <stdio.h>
#include <stdlib.h>
#include "tracker.h"

static Allocation* allocs = NULL;
static size_t count = 0;
static size_t memory = 0;


void* mt_malloc(size_t size){
    void* location = malloc(size);
    if (location == NULL){
        printf("MAlloc failed");
        return NULL;
    }

    Allocation* new_allocs = realloc(allocs, (count + 1) * sizeof(Allocation));
    if (new_allocs == NULL){
        free(location);
        printf("Failed to track allocation");
        return NULL;
    }
    
    allocs = new_allocs;
    allocs[count].ptr = location;
    allocs[count].size = size;
    memory += size;
    count++;
    printf("Allocated %zu bytes to %p (total: %zu)\n", size, location, memory);
    return location;
}

void mt_free(void* ptr){
    if (!ptr){
        printf("invalid input!!!!");
        return;
    }

    for (size_t i =0; i < count; i++){
        if (allocs[i].ptr == ptr){
            memory -= allocs[i].size;

            for (size_t j = i; j < count-1; j++){
                allocs[j] = allocs[j+1];
            }
            count--;
            free(ptr);
            printf("WE freed %p\n", ptr);
            return;
        }
    }
    printf("Pointer %p not found in tracking\n", ptr);
}

int main(){
    int* a = mt_malloc(4 * sizeof(int));
    a[0] = 42;
    printf("%d\n", a[0]);
    mt_free(a);
    return 0;
}