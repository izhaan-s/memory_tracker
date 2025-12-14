#include "tracker.h"
#include <stdio.h>

int main(void){
    int* a = mt_malloc(4 * sizeof(int));
    if (!a) return 1;

    a[0] = 42;
    printf("a[0] = %d\n", a[0]);
    
    mt_print_stats();

    // leaking memory for now to test stats function

    return 0;
}