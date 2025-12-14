#ifndef TRACKER_H
#define TRACKER_H

#include <stddef.h>

typedef enum {MT_LIVE, MT_FREED} State;

// append-only
typedef struct{
    size_t id; // unique allocation instance

    void* ptr;
    size_t size;

    const char* alloc_file;
    int alloc_line;
    const char* free_file;
    int free_line;

    State state;
} Allocation;

// actual implementation
void* mt_malloc_impl(size_t size, const char* file, int line);
void mt_free_impl(void* ptr, const char* file, int line);

// what users call 
#define mt_malloc(size) mt_malloc_impl((size), __FILE__, __LINE__)
#define mt_free(ptr) mt_free_impl((ptr), __FILE__, __LINE__)
void mt_print_stats(void);

#endif