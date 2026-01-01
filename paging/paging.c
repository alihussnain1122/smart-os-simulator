// paging/paging.c - New file for integrated flow functions
#include <stdio.h>
#include "paging.h"

void simulate_paging_flow(int pages[], int n, int frames, int algorithm, int *page_faults) {
    printf("\n--- Paging Simulation ---\n");
    printf("Algorithm: %s\n", algorithm == 1 ? "FIFO" : "LRU");
    printf("Frames: %d, Pages: %d\n", frames, n);
    
    if(algorithm == 1) {
        // FIFO
        *page_faults = fifo_paging_with_count(pages, n, frames);
        // Also show detailed output
        printf("\nDetailed simulation:\n");
        fifo_paging(pages, n, frames);
    } else if(algorithm == 2) {
        // LRU
        *page_faults = lru_paging_with_count(pages, n, frames);
        // Also show detailed output
        printf("\nDetailed simulation:\n");
        lru_paging(pages, n, frames);
    } else {
        printf("Invalid algorithm selection!\n");
        *page_faults = 0;
    }
    
    printf("\nPage faults returned to main flow: %d\n", *page_faults);
}
