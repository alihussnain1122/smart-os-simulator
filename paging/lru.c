// paging/lru.c
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "paging.h"

// Original function (backward compatible)
void lru_paging(int pages[], int n, int frames) {
    int *frame = (int *)malloc(frames * sizeof(int));
    int *last_used = (int *)malloc(frames * sizeof(int));
    int pageFaults = 0;
    
    printf("\n=== LRU Page Replacement Simulation ===\n");
    printf("Frames: %d, Pages: %d\n\n", frames, n);
    
    for(int i = 0; i < frames; i++) {
        frame[i] = -1;
        last_used[i] = -1;
    }
    
    printf("Page\tFrames\t\tPage Fault\n");
    printf("----\t------\t\t----------\n");
    
    for(int i = 0; i < n; i++) {
        int found = 0;
        
        // Check if page is already in frame
        for(int j = 0; j < frames; j++) {
            if(frame[j] == pages[i]) {
                found = 1;
                last_used[j] = i; // Update last used time
                break;
            }
        }
        
        if(!found) {
            // Page fault - need to replace
            int replace_index = -1;
            
            // Find empty frame first
            for(int j = 0; j < frames; j++) {
                if(frame[j] == -1) {
                    replace_index = j;
                    break;
                }
            }
            
            if(replace_index == -1) {
                // No empty frame, find LRU page
                int lru_time = INT_MAX;
                for(int j = 0; j < frames; j++) {
                    if(last_used[j] < lru_time) {
                        lru_time = last_used[j];
                        replace_index = j;
                    }
                }
            }
            
            // Replace the page
            frame[replace_index] = pages[i];
            last_used[replace_index] = i;
            pageFaults++;
            
            printf("%3d\t", pages[i]);
            for(int j = 0; j < frames; j++) {
                if(frame[j] != -1) printf("[%2d] ", frame[j]);
                else printf("[ -] ");
            }
            printf("\tFAULT (%d)\n", pageFaults);
        } else {
            // Page hit
            printf("%3d\t", pages[i]);
            for(int j = 0; j < frames; j++) {
                if(frame[j] != -1) printf("[%2d] ", frame[j]);
                else printf("[ -] ");
            }
            printf("\tHIT\n");
        }
    }
    
    printf("\nTotal Page Faults: %d\n", pageFaults);
    printf("Page Fault Rate: %.2f%%\n", (float)pageFaults / n * 100);
    
    free(frame);
    free(last_used);
}

// New function that returns page fault count
int lru_paging_with_count(int pages[], int n, int frames) {
    int *frame = (int *)malloc(frames * sizeof(int));
    int *last_used = (int *)malloc(frames * sizeof(int));
    int pageFaults = 0;
    
    for(int i = 0; i < frames; i++) {
        frame[i] = -1;
        last_used[i] = -1;
    }
    
    for(int i = 0; i < n; i++) {
        int found = 0;
        
        for(int j = 0; j < frames; j++) {
            if(frame[j] == pages[i]) {
                found = 1;
                last_used[j] = i;
                break;
            }
        }
        
        if(!found) {
            int replace_index = -1;
            
            // Find empty frame
            for(int j = 0; j < frames; j++) {
                if(frame[j] == -1) {
                    replace_index = j;
                    break;
                }
            }
            
            if(replace_index == -1) {
                // Find LRU
                int lru_time = INT_MAX;
                for(int j = 0; j < frames; j++) {
                    if(last_used[j] < lru_time) {
                        lru_time = last_used[j];
                        replace_index = j;
                    }
                }
            }
            
            frame[replace_index] = pages[i];
            last_used[replace_index] = i;
            pageFaults++;
        }
    }
    
    free(frame);
    free(last_used);
    return pageFaults;
}
