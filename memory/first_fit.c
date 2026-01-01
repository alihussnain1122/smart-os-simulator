#include <stdio.h>
#include <unistd.h>
#include "memory.h"

void first_fit(Block blocks[], int b, MemProcess p[], int n) {
    int totalInternalFrag = 0;

    printf("\nFirst Fit Memory Allocation\n");

    for (int i = 0; i < n; i++) {
        p[i].blockIndex = -1;

        for (int j = 0; j < b; j++) {
            if (!blocks[j].allocated && blocks[j].size >= p[i].size) {
                p[i].blockIndex = j;
                blocks[j].allocated = 1;

                totalInternalFrag += (blocks[j].size - p[i].size);
                break;
            }
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
