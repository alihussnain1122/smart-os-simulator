#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include "memory.h"

void best_fit(Block blocks[], int b, MemProcess p[], int n) {
    int totalInternalFrag = 0;

    printf("\n Best Fit Memory Allocation\n");

    for (int i = 0; i < n; i++) {
        int bestIndex = -1;
        int minWaste = INT_MAX;

        for (int j = 0; j < b; j++) {
            if (!blocks[j].allocated && blocks[j].size >= p[i].size) {
                int waste = blocks[j].size - p[i].size;
                if (waste < minWaste) {
                    minWaste = waste;
                    bestIndex = j;
                }
            }
        }

        if (bestIndex != -1) {
            p[i].blockIndex = bestIndex;
            blocks[bestIndex].allocated = 1;
            totalInternalFrag += minWaste;
        } else {
            p[i].blockIndex = -1;
        }

        // Stepwise print
        printf("Process %d: ", p[i].pid);
        if (p[i].blockIndex != -1)
            printf("Allocated to Block %d", p[i].blockIndex + 1);
        else
            printf("Not Allocated");
        printf("\n");
        sleep(1);
    }

    printf("\nPID\tProcessSize\tBlockAllocated\n");
    for (int i = 0; i < n; i++) {
        if (p[i].blockIndex != -1)
            printf("%d\t%d\t\t%d\n", p[i].pid, p[i].size, p[i].blockIndex + 1);
        else
            printf("%d\t%d\t\tNot Allocated\n", p[i].pid, p[i].size);
    }

    printf("\nTotal Internal Fragmentation = %d\n", totalInternalFrag);
}
