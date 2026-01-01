#ifndef CPU_H
#define CPU_H

typedef struct {
    int pid;
    int arrival;
    int burst;
    int remaining;
    int waiting;
    int turnaround;
} Process;

void fcfs(Process p[], int n);
void sjf(Process p[], int n);
void round_robin(Process p[], int n, int quantum);

#endif
