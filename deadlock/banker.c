#include <stdio.h>
#include <unistd.h> 
#include "banker.h"

void bankers_algorithm(int n, int m,
                        int alloc[n][m],
                        int max[n][m],
                        int avail[m]) {

    int need[n][m];
    int finish[n];
    int safeSeq[n];
    int work[m];

    // Need = Max - Allocation
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            need[i][j] = max[i][j] - alloc[i][j];

    for (int i = 0; i < n; i++)
        finish[i] = 0;

    for (int j = 0; j < m; j++)
        work[j] = avail[j];

    int count = 0;

    printf("\n Banker's Algorithm\n");

    while (count < n) {
        int found = 0;

        for (int i = 0; i < n; i++) {
            if (!finish[i]) {
                int j;
                for (j = 0; j < m; j++) {
                    if (need[i][j] > work[j])
                        break;
                }

                printf("Checking P%d: ", i);
                if (j == m) {
                    printf("Can be allocated safely\n");
                    for (int k = 0; k < m; k++)
                        work[k] += alloc[i][k];

                    safeSeq[count++] = i;
                    finish[i] = 1;
                    found = 1;
                } else {
                    printf("Cannot allocate, resources insufficient\n");
                }

                sleep(1); 
            }
        }

        if (!found) {
            printf("\nSystem is NOT in a safe state (Deadlock possible)\n");
            return;
        }
    }

    printf("\nSystem is in a SAFE state.\nSafe sequence: ");
    for (int i = 0; i < n; i++)
        printf("P%d ", safeSeq[i]);
    printf("\n");
}
