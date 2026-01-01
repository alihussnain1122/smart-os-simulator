#include <stdio.h>
#include <unistd.h>
#include "cpu.h"

void fcfs(Process p[], int n) {
    int time = 0;
    float totalWT = 0, totalTAT = 0;

    printf("\nFCFS Scheduling\n");
    printf("Gantt Chart:\n|");

    for (int i = 0; i < n; i++) {
        if (time < p[i].arrival)
            time = p[i].arrival;

        p[i].waiting = time - p[i].arrival;
        time += p[i].burst;
        p[i].turnaround = p[i].waiting + p[i].burst;

        totalWT += p[i].waiting;
        totalTAT += p[i].turnaround;
        // Stepwise execution animation
        printf(" P%d ", p[i].pid); fflush(stdout);
        for(int j=0;j<3;j++){ printf("."); fflush(stdout); sleep(1); } 
        printf("|");
    }
    printf("\n\n" "PID\tAT\tBT\tWT\tTAT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\n",
               p[i].pid,
               p[i].arrival,
               p[i].burst,
               p[i].waiting,
               p[i].turnaround);
        sleep(1); // pause between printing each process
    }
    printf("\nAverage Waiting Time = %.2f\n", totalWT / n);
    printf("Average Turnaround Time = %.2f\n", totalTAT / n);
}
