// paging/fifo.c
#include <stdio.h>
#include <stdlib.h>
#include "paging.h"

// Original function (backward compatible)
void fifo_paging(int pages[], int n, int frames) {
    int *frame = (int *)malloc(frames * sizeof(int));
    int *arrival_time = (int *)malloc(frames * sizeof(int));
    int index = 0;
    int pageFaults = 0;
    int time = 0;
    
    printf("\n=== FIFO Page Replacement Simulation ===\n");
    printf("Frames: %d, Pages: %d\n\n", frames, n);
    
    for(int i = 0; i < frames; i++) {
        frame[i] = -1;
        arrival_time[i] = -1;
    }
    
    printf("Page\tFrames\t\tPage Fault\n");
    printf("----\t------\t\t----------\n");
    
    for(int i = 0; i < n; i++) {
        int found = 0;
        
        // Check if page is already in frame
        for(int j = 0; j < frames; j++) {
            if(frame[j] == pages[i]) {
                found = 1;
                break;
            }
        }
        
        if(!found) {
            // Page fault - need to replace
            if(time < frames) {
                // Empty frame available
                frame[time] = pages[i];
                arrival_time[time] = time;
            } else {
                // Find the oldest page (FIFO)
                int oldest_time = arrival_time[0];
                int oldest_index = 0;
                
                for(int j = 1; j < frames; j++) {
                    if(arrival_time[j] < oldest_time) {
                        oldest_time = arrival_time[j];
                        oldest_index = j;
                    }
                }
                
                // Replace the oldest page
                frame[oldest_index] = pages[i];
                arrival_time[oldest_index] = time;
            }
            pageFaults++;
            time++;
            
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
    free(arrival_time);
}

// New function that returns page fault count
int fifo_paging_with_count(int pages[], int n, int frames) {
    int *frame = (int *)malloc(frames * sizeof(int));
    int *arrival_time = (int *)malloc(frames * sizeof(int));
    int pageFaults = 0;
    int time = 0;
    
    for(int i = 0; i < frames; i++) {
        frame[i] = -1;
        arrival_time[i] = -1;
    }
    
    for(int i = 0; i < n; i++) {
        int found = 0;
        
        for(int j = 0; j < frames; j++) {
            if(frame[j] == pages[i]) {
                found = 1;
                break;
            }
        }
        
        if(!found) {
            if(time < frames) {
                frame[time] = pages[i];
                arrival_time[time] = time;
            } else {
                int oldest_time = arrival_time[0];
                int oldest_index = 0;
                
                for(int j = 1; j < frames; j++) {
                    if(arrival_time[j] < oldest_time) {
                        oldest_time = arrival_time[j];
                        oldest_index = j;
                    }
                }
                
                frame[oldest_index] = pages[i];
                arrival_time[oldest_index] = time;
            }
            pageFaults++;
            time++;
        }
    }
    
    free(frame);
    free(arrival_time);
    return pageFaults;
}
