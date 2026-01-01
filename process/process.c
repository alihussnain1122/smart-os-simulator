// process/process.c
// Demonstrates process creation (fork, exec), threads, IPC (shared memory), and synchronization (semaphores)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include "process.h"

#define SHM_SIZE 1024

sem_t proc_mutex;
int thread_iterations = 5;

void* thread_func(void* arg) {
    int* shared_var = (int*)arg;
    for(int i = 0; i < thread_iterations; i++) {
        sem_wait(&proc_mutex);
        (*shared_var)++;
        printf("[Thread] Incremented shared_var to %d\n", *shared_var);
        sem_post(&proc_mutex);
        sleep(1);
    }
    return NULL;
}

void process_demo() {
    char exec_cmd[100];
    char exec_arg[50];
    int initial_value;
    
    printf("\n=== PROCESS CREATION MODULE ===\n");
    printf("Demonstrates: fork(), exec(), threads, shared memory, semaphores\n\n");
    
    printf("Enter initial value for shared variable: ");
    scanf("%d", &initial_value);
    
    printf("Enter number of thread iterations: ");
    scanf("%d", &thread_iterations);
    if(thread_iterations < 1) {
        printf("Invalid input. Please enter a positive value.\n");
        return;
    }
    
    printf("Enter command to execute in child process (e.g., /bin/ls): ");
    scanf("%s", exec_cmd);
    
    printf("Enter argument for the command (e.g., -l or . ): ");
    scanf("%s", exec_arg);

    printf("\nSetting up shared memory...\n");
    key_t key = ftok("process.c", 65);
    int shmid = shmget(key, SHM_SIZE, 0666|IPC_CREAT);
    if(shmid < 0) { perror("shmget"); return; }
    int* shared_var = (int*)shmat(shmid, NULL, 0);
    *shared_var = initial_value;
    printf("Shared memory created. Initial value: %d\n", *shared_var);

    // Semaphore init
    sem_init(&proc_mutex, 0, 1);

    printf("\nForking child process...\n");
    pid_t pid = fork();
    if(pid < 0) {
        perror("fork");
        return;
    }
    else if(pid == 0) {
        // Child process
        printf("[Child Process] PID: %d\n", getpid());
        printf("[Child Process] Executing '%s %s'...\n", exec_cmd, exec_arg);
        execl(exec_cmd, exec_cmd, exec_arg, NULL);
        perror("execl failed");
        exit(1);
    } else {
        // Parent process
        printf("[Parent Process] PID: %d, waiting for child...\n", getpid());
        wait(NULL);
        printf("\n[Parent Process] Child finished.\n");

        // Thread demo
        pthread_t tid;
        printf("[Parent Process] Creating thread to increment shared_var %d times...\n", thread_iterations);
        pthread_create(&tid, NULL, thread_func, shared_var);
        pthread_join(tid, NULL);

        printf("\n[Parent Process] Final shared_var value: %d\n", *shared_var);

        // IPC demo: shared memory
        printf("[Parent Process] Value in shared memory: %d\n", *shared_var);
        
        // Simulate another process reading
        int* shm_read = (int*)shmat(shmid, NULL, 0);
        printf("[Simulated Reader] Read from shared memory: %d\n", *shm_read);
        shmdt(shm_read);

        // Cleanup
        shmdt(shared_var);
        shmctl(shmid, IPC_RMID, NULL);
        sem_destroy(&proc_mutex);
        printf("\n[Parent Process] Shared memory cleaned up. Operation complete.\n");
    }
}
