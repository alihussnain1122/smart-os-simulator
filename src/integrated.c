// src/integrated.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/wait.h>
#include "integrated.h"

#define MAX_THREADS 64
static sem_t int_mutex;

static void* int_cpu_thread_func(void* arg) {
    Process* p = (Process*)arg;
    sem_wait(&int_mutex);
    printf("[Thread] Simulating process %d (CPU)\n", p->pid);
    sleep(1);
    sem_post(&int_mutex);
    pthread_exit(NULL);
}

static void* int_mem_thread_func(void* arg) {
    MemProcess* mp = (MemProcess*)arg;
    sem_wait(&int_mutex);
    printf("[Thread] Simulating memory allocation for process %d\n", mp->pid);
    sleep(1);
    sem_post(&int_mutex);
    pthread_exit(NULL);
}

static void* int_paging_thread_func(void* arg) {
    int* page = (int*)arg;
    sem_wait(&int_mutex);
    printf("[Thread] Simulating page reference %d\n", *page);
    sleep(1);
    sem_post(&int_mutex);
    pthread_exit(NULL);
}

void integrated_flow_mode() {
    printf(CYAN "\n========== INTEGRATED FLOW MODE ==========\n" RESET);
    printf("Flow: Input -> CPU -> Memory -> Paging -> Deadlock -> Output\n\n");
    
    // Initialize semaphore
    sem_init(&int_mutex, 0, 1);
    
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    
    if(n <= 0) {
        printf(RED "Invalid number of processes!\n" RESET);
        sem_destroy(&int_mutex);
        return;
    }
    
    IntegratedProcess processes[n];
    pthread_t cpu_tids[n];
    pthread_t mem_tids[n];
    pthread_t paging_tids[n];
    
    // 1. INPUT PHASE
    printf(YELLOW "\n[PHASE 1] INPUT COLLECTION\n" RESET);
    printf("================================\n");
    
    int total_memory_needed = 0;
    for(int i = 0; i < n; i++) {
        processes[i].pid = i + 1;
        processes[i].completed = 0;
        
        printf("\n" BLUE "Process P%d:\n" RESET, i+1);
        
        // CPU data
        printf("  Arrival Time: ");
        scanf("%d", &processes[i].arrival_time);
        printf("  Burst Time: ");
        scanf("%d", &processes[i].burst_time);
        processes[i].remaining_time = processes[i].burst_time;
        
        // Memory data
        printf("  Memory Required (KB): ");
        scanf("%d", &processes[i].memory_required);
        total_memory_needed += processes[i].memory_required;
        
        // Paging data
        printf("  Number of Page References: ");
        scanf("%d", &processes[i].num_pages);
        processes[i].page_references = (int*)malloc(processes[i].num_pages * sizeof(int));
        
        if(processes[i].num_pages > 0) {
            printf("  Enter page reference string (space-separated): ");
            for(int j = 0; j < processes[i].num_pages; j++) {
                scanf("%d", &processes[i].page_references[j]);
            }
        } else {
            // Default page references if none provided
            processes[i].num_pages = 5;
            processes[i].page_references = (int*)malloc(5 * sizeof(int));
            int default_pages[] = {1, 2, 3, 2, 1};
            memcpy(processes[i].page_references, default_pages, 5 * sizeof(int));
        }
        
        // Deadlock data
        printf("  Number of Resource Types: ");
        scanf("%d", &processes[i].num_resources);
        
        if(processes[i].num_resources > 0) {
            processes[i].allocation = (int*)calloc(processes[i].num_resources, sizeof(int));
            processes[i].max_need = (int*)calloc(processes[i].num_resources, sizeof(int));
            processes[i].current_need = (int*)calloc(processes[i].num_resources, sizeof(int));
            
            printf("  Current Allocation (space-separated, %d values): ", processes[i].num_resources);
            for(int j = 0; j < processes[i].num_resources; j++) {
                scanf("%d", &processes[i].allocation[j]);
            }
            
            printf("  Maximum Need (space-separated, %d values): ", processes[i].num_resources);
            for(int j = 0; j < processes[i].num_resources; j++) {
                scanf("%d", &processes[i].max_need[j]);
                processes[i].current_need[j] = processes[i].max_need[j] - processes[i].allocation[j];
            }
        } else {
            // Default resource allocation if none provided
            processes[i].num_resources = 2;
            processes[i].allocation = (int*)calloc(2, sizeof(int));
            processes[i].max_need = (int*)calloc(2, sizeof(int));
            processes[i].current_need = (int*)calloc(2, sizeof(int));
            processes[i].allocation[0] = 1; // Resource 1
            processes[i].max_need[0] = 2;
            processes[i].current_need[0] = 1;
        }
    }
    
    // AUTOMATIC ALGORITHM SELECTION
    printf(MAGENTA "\n[AUTOMATIC ALGORITHM SELECTION]\n" RESET);
    printf("================================\n");

    // --- CPU Scheduling Heuristic ---
    int max_burst = 0, min_burst = 1e9;
    for(int i = 0; i < n; i++){
        if(processes[i].burst_time > max_burst) max_burst = processes[i].burst_time;
        if(processes[i].burst_time < min_burst) min_burst = processes[i].burst_time;
    }

    int cpu_algo, quantum = 2; // default quantum for RR
    if(max_burst - min_burst > 5) cpu_algo = 2; // SJF
    else cpu_algo = 1; // FCFS
    printf("Selected CPU Scheduling Algorithm: %s\n", (cpu_algo==1)?"FCFS":"SJF");

    // --- Memory Allocation Heuristic ---
    int num_blocks;
    printf("\nEnter number of memory blocks: ");
    scanf("%d", &num_blocks);

    int mem_algo;
    int total_block_size = 0;
    Block blocks[num_blocks];
    for(int i = 0; i < num_blocks; i++){
        printf("Enter block size %d: ", i+1);
        scanf("%d", &blocks[i].size);
        blocks[i].allocated = 0;
        total_block_size += blocks[i].size;
    }

    if(total_memory_needed < total_block_size/2) mem_algo = 1; // First Fit
    else if(total_memory_needed < total_block_size) mem_algo = 2; // Best Fit
    else mem_algo = 3; // Worst Fit

    printf("Selected Memory Allocation Algorithm: %s\n", 
        (mem_algo==1)?"First Fit":(mem_algo==2)?"Best Fit":"Worst Fit");

    // --- Paging Heuristic ---
    int frames;
    printf("\nEnter number of frames for paging: ");
    scanf("%d", &frames);

    int total_refs = 0;
    for(int i = 0; i < n; i++) total_refs += processes[i].num_pages;
    int avg_refs = total_refs / n;

    int paging_algo;
    if(avg_refs <= frames/2) paging_algo = 1; // FIFO
    else paging_algo = 2; // LRU

    printf("Selected Paging Algorithm: %s\n", (paging_algo==1)?"FIFO":"LRU");

    sleep(2);

    // ------------------ CPU SCHEDULING ------------------
    printf(GREEN "\n[PHASE 2] CPU SCHEDULING\n" RESET);
    Process cpu_processes[n];
    for(int i = 0; i < n; i++){
        cpu_processes[i].pid = processes[i].pid;
        cpu_processes[i].arrival = processes[i].arrival_time;
        cpu_processes[i].burst = processes[i].burst_time;
        cpu_processes[i].remaining = processes[i].remaining_time;
        pthread_create(&cpu_tids[i], NULL, int_cpu_thread_func, &cpu_processes[i]);
    }
    for(int i = 0; i < n; i++) pthread_join(cpu_tids[i], NULL);
    if(cpu_algo == 1) fcfs(cpu_processes, n);
    else if(cpu_algo == 2) sjf(cpu_processes, n);
    else round_robin(cpu_processes, n, quantum);
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(cpu_processes[j].pid == processes[i].pid){
                processes[i].waiting_time = cpu_processes[j].waiting;
                processes[i].turnaround_time = cpu_processes[j].turnaround;
                processes[i].completed = 1;
                break;
            }
        }
    }
    sleep(2);

    // ------------------ MEMORY ------------------
    printf(YELLOW "\n[PHASE 3] MEMORY MANAGEMENT\n" RESET);
    MemProcess mem_processes[n];
    for(int i = 0; i < n; i++){
        mem_processes[i].pid = processes[i].pid;
        mem_processes[i].size = processes[i].memory_required;
        mem_processes[i].blockIndex = -1;
        pthread_create(&mem_tids[i], NULL, int_mem_thread_func, &mem_processes[i]);
    }
    for(int i = 0; i < n; i++) pthread_join(mem_tids[i], NULL);
    if(mem_algo == 1) first_fit(blocks, num_blocks, mem_processes, n);
    else if(mem_algo == 2) best_fit(blocks, num_blocks, mem_processes, n);
    else worst_fit(blocks, num_blocks, mem_processes, n);
    for(int i = 0; i < n; i++){
        processes[i].allocated_block = mem_processes[i].blockIndex;
        processes[i].memory_allocated = (mem_processes[i].blockIndex != -1);
    }
    sleep(2);

    // ------------------ PAGING ------------------
    printf(BLUE "\n[PHASE 4] PAGING SIMULATION\n" RESET);
    for(int i = 0; i < n; i++){
        if(processes[i].num_pages > 0){
            for(int j = 0; j < processes[i].num_pages; j++) {
                pthread_create(&paging_tids[j], NULL, int_paging_thread_func, &processes[i].page_references[j]);
            }
            for(int j = 0; j < processes[i].num_pages; j++) pthread_join(paging_tids[j], NULL);
            printf("\n--- Process P%d Paging (%d references) ---\n",
                processes[i].pid, processes[i].num_pages);
            if(paging_algo == 1)
                processes[i].page_faults = fifo_paging_with_count(
                    processes[i].page_references,
                    processes[i].num_pages,
                    frames
                );
            else
                processes[i].page_faults = lru_paging_with_count(
                    processes[i].page_references,
                    processes[i].num_pages,
                    frames
                );
            printf("Process P%d total page faults: %d\n",
                processes[i].pid, processes[i].page_faults);
        }
    }
    sleep(2);

    // ------------------ DEADLOCK ------------------
    printf(RED "\n[PHASE 5] DEADLOCK DETECTION\n" RESET);
    int max_resources = 0;
    for(int i = 0; i < n; i++) if(processes[i].num_resources > max_resources) max_resources = processes[i].num_resources;
    if(max_resources > 0 && n > 0){
        int alloc[n][max_resources], max[n][max_resources], avail[max_resources];
        for(int i = 0; i < n; i++){
            for(int j = 0; j < max_resources; j++){
                if(j < processes[i].num_resources){
                    alloc[i][j] = processes[i].allocation[j];
                    max[i][j] = processes[i].max_need[j];
                } else {
                    alloc[i][j] = 0;
                    max[i][j] = 0;
                }
            }
        }
        printf("Enter Available Resources (%d values, space-separated): ", max_resources);
        for(int j = 0; j < max_resources; j++) scanf("%d", &avail[j]);
        printf("\nRunning Banker's Algorithm for system safety...\n");
        pid_t pid = fork();
        if(pid == 0) {
            bankers_algorithm(n, max_resources, alloc, max, avail);
            exit(0);
        } else {
            wait(NULL);
        }
    } else printf("Skipping deadlock detection (no resources specified)\n");
    sleep(2);

    // ------------------ FINAL REPORT ------------------
    print_integrated_report(processes, n);
    for(int i = 0; i < n; i++){
        free(processes[i].page_references);
        if(processes[i].num_resources > 0){
            free(processes[i].allocation);
            free(processes[i].max_need);
            free(processes[i].current_need);
        }
    }
    sem_destroy(&int_mutex);
}
