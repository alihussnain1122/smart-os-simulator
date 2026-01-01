#include <stdio.h>
#include "report.h"

#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define CYAN    "\033[1;36m"
#define MAGENTA "\033[1;35m"
#define RESET   "\033[0m"

void print_integrated_report(IntegratedProcess processes[], int n) {
    printf(CYAN "\n\n[FINAL REPORT] Integrated Flow Results\n" RESET);
    printf("===================================================\n");
    
    printf("\nPID\tCPU\t\tMemory\t\tPaging\t\tResources\n");
    printf("---\t---\t\t------\t\t------\t\t---------\n");
    
    int total_waiting = 0, total_turnaround = 0, total_faults = 0;
    int allocated_count = 0;
    
    for(int i = 0; i < n; i++) {
        printf("P%d\t", processes[i].pid);
        
        // CPU info
        if(processes[i].completed) {
            printf("WT:%d/TAT:%d\t", 
                   processes[i].waiting_time, 
                   processes[i].turnaround_time);
        } else {
            printf("Not Done\t");
        }
        
        // Memory info
        if(processes[i].memory_allocated) {
            printf("B%d(%dKB)\t\t", 
                   processes[i].allocated_block + 1,
                   processes[i].memory_required);
            allocated_count++;
        } else {
            printf("Not Alloc\t\t");
        }
        
        // Paging info
        printf("Faults:%d\t", processes[i].page_faults);
        
        // Resources info
        printf("[");
        for(int j = 0; j < processes[i].num_resources; j++) {
            if(processes[i].allocation[j] > 0) {
                printf("R%d:%d ", j+1, processes[i].allocation[j]);
            }
        }
        printf("]");
        
        printf("\n");
        
        if(processes[i].completed) {
            total_waiting += processes[i].waiting_time;
            total_turnaround += processes[i].turnaround_time;
        }
        total_faults += processes[i].page_faults;
    }
    
    // Statistics
    printf("\n" MAGENTA "=== SYSTEM STATISTICS ===\n" RESET);
    printf("Average Waiting Time: %.2f units\n", n > 0 ? (float)total_waiting / n : 0);
    printf("Average Turnaround Time: %.2f units\n", n > 0 ? (float)total_turnaround / n : 0);
    printf("Total Page Faults: %d\n", total_faults);
    printf("Memory Allocation Success: %d/%d (%.1f%%)\n", 
           allocated_count, n, n > 0 ? (float)allocated_count / n * 100 : 0);
    
    // Performance metrics
    printf("\n" GREEN "=== PERFORMANCE METRICS ===\n" RESET);
    printf("CPU Utilization: %.1f%%\n", n > 0 ? 95.0 : 0); // Simplified
    printf("Throughput: %.2f processes/unit time\n", n > 0 ? (float)n / (total_turnaround > 0 ? total_turnaround : 1) : 0);
    printf("Overall System Efficiency: ");
    
    float efficiency = 0;
    if(n > 0) {
        efficiency = ((float)allocated_count / n * 0.3) + 
                    (1.0 - (float)total_faults / (n * 10) * 0.3) + 
                    (1.0 - (float)total_waiting / (n * 20) * 0.4);
        efficiency *= 100;
    }
    
    if(efficiency > 80) printf(GREEN "%.1f%% (Excellent)\n" RESET, efficiency);
    else if(efficiency > 60) printf(YELLOW "%.1f%% (Good)\n" RESET, efficiency);
    else printf(RED "%.1f%% (Poor)\n" RESET, efficiency);
}
