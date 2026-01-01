// src/individual.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/wait.h>
#include "individual.h"
#include "cpu/cpu.h"
#include "memory/memory.h"
#include "paging/paging.h"
#include "deadlock/banker.h"
#include "threads/threads.h"
#include "ipc/shm.h"
#include "process/process.h"

// Colors
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN    "\033[1;36m"
#define RESET   "\033[0m"

#define SHM_KEY 0x1234
#define MAX_PROCESSES 32
static sem_t ind_mutex;

typedef struct {
    int pid;
    int result;
} ThreadResult;

static void* ind_cpu_thread_func(void* arg) {
    Process* p = (Process*)arg;
    sem_wait(&ind_mutex);
    printf("[Thread] Simulating process %d\n", p->pid);
    sleep(1);
    sem_post(&ind_mutex);
    pthread_exit(NULL);
}

void individual_module_testing() {
    char choice;
    printf("\n========== INDIVIDUAL MODULE TESTING ==========");
    printf("\n[C] CPU Scheduler");
    printf("\n[M] Memory Manager");
    printf("\n[P] Paging Unit");
    printf("\n[D] Deadlock Detector");
    printf("\n[T] Threads & Synchronization");
    printf("\n[I] IPC (Shared Memory)");
    printf("\n[F] Process Creation (fork/exec)");
    printf("\nSelect Module: ");
    scanf(" %c", &choice);

    sem_init(&ind_mutex, 0, 1);

    if(choice == 'C' || choice == 'c') {
        int n, algo, quantum;
        printf("\n[CPU SCHEDULER MODE]\nEnter number of processes: ");
        scanf("%d", &n);
        Process p[n];
        pthread_t tids[n];
        for(int i=0;i<n;i++){
            p[i].pid = i+1;
            printf("Process %d Arrival Time: ", i+1);
            scanf("%d", &p[i].arrival);
            printf("Process %d Burst Time: ", i+1);
            scanf("%d", &p[i].burst);
            p[i].remaining = p[i].burst;
        }
        printf("\nChoose Algorithm:\n1. FCFS\n2. SJF (Non-Preemptive)\n3. Round Robin\nChoice: ");
        scanf("%d", &algo);
        printf("Processing");
        for(int i=0;i<5;i++){ printf("."); fflush(stdout); sleep(1);}
        printf("\n");
        // Launch a thread for each process
        for(int i=0;i<n;i++) pthread_create(&tids[i], NULL, ind_cpu_thread_func, &p[i]);
        for(int i=0;i<n;i++) pthread_join(tids[i], NULL);
        if(algo==1) fcfs(p,n);
        else if(algo==2) sjf(p,n);
        else if(algo==3){
            printf("Enter Time Quantum: "); scanf("%d",&quantum);
            round_robin(p,n,quantum);
        }
        else printf("Invalid Algorithm!\n");
    }
    else if(choice == 'M' || choice == 'm'){
        int b,n,algoChoice;
        printf("\n[MEMORY MANAGER MODE]\nEnter number of memory blocks: "); scanf("%d",&b);
        Block blocks[b];
        pthread_t tids[b];
        for(int i=0;i<b;i++){
            printf("Size of Block %d: ", i+1); scanf("%d",&blocks[i].size);
            blocks[i].allocated = 0;
        }
        printf("Enter number of processes: "); scanf("%d",&n);
        MemProcess p[n];
        for(int i=0;i<n;i++){
            p[i].pid = i+1;
            printf("Memory required by Process %d: ", i+1); scanf("%d",&p[i].size);
        }
        printf("\nChoose Allocation Algorithm:\n1. First Fit\n2. Best Fit\n3. Worst Fit\nChoice: "); scanf("%d",&algoChoice);
        printf("Allocating memory");
        for(int i=0;i<5;i++){ printf("."); fflush(stdout); sleep(1);}
        printf("\n");
        // Launch a thread for each block
        for(int i=0;i<b;i++) pthread_create(&tids[i], NULL, ind_cpu_thread_func, &blocks[i]);
        for(int i=0;i<b;i++) pthread_join(tids[i], NULL);
        if(algoChoice==1) first_fit(blocks,b,p,n);
        else if(algoChoice==2) best_fit(blocks,b,p,n);
        else if(algoChoice==3) worst_fit(blocks,b,p,n);
        else printf(RED "Invalid Algorithm!\n" RESET);
    }
    else if(choice == 'P' || choice == 'p'){
        int n,frames,pagingChoice;
        printf("\n[PAGING UNIT MODE]\nEnter number of pages: "); scanf("%d",&n);
        int pages[n];
        pthread_t tids[n];
        printf("Enter page reference string:\n");
        for(int i=0;i<n;i++) scanf("%d",&pages[i]);
        printf("Enter number of frames: "); scanf("%d",&frames);
        printf("\nChoose Page Replacement Algorithm:\n1. FIFO\n2. LRU\nChoice: "); scanf("%d",&pagingChoice);
        printf("Simulating paging");
        for(int i=0;i<5;i++){ printf("."); fflush(stdout); sleep(1);}
        printf("\n");
        // Launch a thread for each page reference
        for(int i=0;i<n;i++) pthread_create(&tids[i], NULL, ind_cpu_thread_func, &pages[i]);
        for(int i=0;i<n;i++) pthread_join(tids[i], NULL);
        if(pagingChoice==1) fifo_paging(pages,n,frames);
        else if(pagingChoice==2) lru_paging(pages,n,frames);
        else printf(RED "Invalid Algorithm!\n" RESET);
    }
    else if(choice == 'D' || choice == 'd'){
        int n,m;
        printf("\n[DEADLOCK DETECTOR MODE]\nEnter number of processes: "); scanf("%d",&n);
        printf("Enter number of resource types: "); scanf("%d",&m);
        int alloc[n][m], max[n][m], avail[m];
        printf("Enter Allocation Matrix:\n");
        for(int i=0;i<n;i++)
            for(int j=0;j<m;j++)
                scanf("%d",&alloc[i][j]);
        printf("Enter Max Matrix:\n");
        for(int i=0;i<n;i++)
            for(int j=0;j<m;j++)
                scanf("%d",&max[i][j]);
        printf("Enter Available Resources:\n");
        for(int j=0;j<m;j++) scanf("%d",&avail[j]);
        printf("Checking for safe sequence");
        for(int i=0;i<5;i++){ printf("."); fflush(stdout); sleep(1);}
        printf("\n");
        pid_t pid = fork();
        if(pid == 0) {
            bankers_algorithm(n,m,alloc,max,avail);
            exit(0);
        } else {
            wait(NULL);
        }
    }
    else if(choice == 'T' || choice == 't'){
        threads_demo();
    }
    else if(choice == 'I' || choice == 'i'){
        shm_demo();
    }
    else if(choice == 'F' || choice == 'f'){
        process_demo();
    }
    else printf("Invalid Module Selection!\n");
    sem_destroy(&ind_mutex);
}
