// paging/paging.h
#ifndef PAGING_H
#define PAGING_H

// Original functions (for backward compatibility)
void fifo_paging(int pages[], int n, int frames);
void lru_paging(int pages[], int n, int frames);

// New functions that return page fault count
int fifo_paging_with_count(int pages[], int n, int frames);
int lru_paging_with_count(int pages[], int n, int frames);

// Integrated flow function
void simulate_paging_flow(int pages[], int n, int frames, int algorithm, int *page_faults);

#endif
