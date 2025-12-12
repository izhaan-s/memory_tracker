#ifndef TRACKER_H
#define TRACKER_H

#include <stddef.h>

typedef struct{
    void* ptr;
    size_t size;
} Allocation;

void* mt_malloc(size_t size);

void mt_free(void* ptr);

#endif
