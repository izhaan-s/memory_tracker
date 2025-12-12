#include <stdio.h>
#include <stdlib.h>

typedef struct{
    void* ptr;
    size_t size;
} Allocation;

static Allocation* allocs = NULL;
static size_t count = 0;
static size_t memory = 0;


void* mt_malloc(size_t size){
    void* location = malloc(size);

    if (location == NULL){
        printf("MAlloc failed");
        return NULL;
    }

    printf("Allocated %zu bytes to %p\n", size, location);
    return location;
}

void mt_free(void* ptr){
    if (!ptr){
        printf("invalid input!!!!");
        return;
    }
    printf("WE are freeing %p\n", ptr);
    free(ptr);
}

int main(){
    int* a = mt_malloc(4 * sizeof(int));
    a[0] = 42;
    printf("%d\n", a[0]);
    mt_free(a);
    return 0;
}