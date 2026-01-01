#include <stdio.h>
#include <unistd.h> 
#include "cpu.h"

void round_robin(Process p[], int n, int quantum) {
    int time = 0, completed = 0;
    float totalWT = 0, totalTAT = 0;

    printf("\nRound Robin Scheduling\n");
    printf("Time Quantum = %d\n", quantum);
    printf("Gantt Chart:\n|");

    while (completed < n) {
        int doneSomething = 0;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= time && p[i].remaining > 0) {
                doneSomething = 1;

                // Stepwise animation
                printf(" P%d ", p[i].pid); fflush(stdout);
                for(int j=0;j<3;j++){ printf("."); fflush(stdout); sleep(1); }
                printf("|");

                if (p[i].remaining > quantum) {
                    time += quantum;
                    p[i].remaining -= quantum;
                } else {
                    time += p[i].remaining;
                    p[i].remaining = 0;
                    completed++;

                    p[i].turnaround = time - p[i].arrival;
                    p[i].waiting = p[i].turnaround - p[i].burst;

                    totalWT += p[i].waiting;
                    totalTAT += p[i].turnaround;
                }
            }
        }

        // CPU idle case
        if (!doneSomething) {
            printf(" idle "); fflush(stdout);
            for(int j=0;j<2;j++){ printf("."); fflush(stdout); sleep(1);}
            printf("|");
            time++;
        }
    }

    // Print final table
    printf("\n\n" "PID\tAT\tBT\tWT\tTAT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\n",
               p[i].pid,
               p[i].arrival,
               p[i].burst,
               p[i].waiting,
               p[i].turnaround);
        sleep(1); 
    }

    printf("\nAverage Waiting Time = %.2f\n", totalWT / n);
    printf("Average Turnaround Time = %.2f\n", totalTAT / n);
}
