#include <stdio.h>
#include <limits.h>
#include <unistd.h> 
#include "cpu.h"

void sjf(Process p[], int n) {
    int completed = 0, time = 0;
    int isDone[n];
    float totalWT = 0, totalTAT = 0;

    for (int i = 0; i < n; i++)
        isDone[i] = 0;

    printf("\n SJF (Non-Preemptive) Scheduling\n");
    printf("Gantt Chart:\n|");

    while (completed < n) {
        int idx = -1;
        int minBurst = INT_MAX;

        // Find shortest job available at current time
        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= time && !isDone[i]) {
                if (p[i].burst < minBurst) {
                    minBurst = p[i].burst;
                    idx = i;
                }
            }
        }

        // CPU idle case
        if (idx == -1) {
            printf(" idle "); fflush(stdout);
            for(int j=0;j<2;j++){ printf("."); fflush(stdout); sleep(1); }
            printf("|");
            time++;
            continue;
        }

        p[idx].waiting = time - p[idx].arrival;
        time += p[idx].burst;
        p[idx].turnaround = p[idx].waiting + p[idx].burst;

        totalWT += p[idx].waiting;
        totalTAT += p[idx].turnaround;

        isDone[idx] = 1;
        completed++;

        // Stepwise animation for Gantt chart
        printf(" P%d ", p[idx].pid); fflush(stdout);
        for(int j=0;j<3;j++){ printf("."); fflush(stdout); sleep(1); }
        printf("|");
    }

    // Print table
    printf("\n\n"  "PID\tAT\tBT\tWT\tTAT\n");
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
