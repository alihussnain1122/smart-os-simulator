// ipc/shm.c
// Demonstrates shared memory IPC, process creation, threads, and synchronization
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include "shm.h"

#define SHM_SIZE 256

sem_t shm_mutex;
int num_messages = 3;
int num_reads = 4;

void* thread_writer(void* arg) {
    char* shm_ptr = (char*)arg;
    for(int i = 0; i < num_messages; i++) {
        sem_wait(&shm_mutex);
        sprintf(shm_ptr, "Message %d from parent thread", i+1);
        printf("[Parent Thread] Wrote to shared memory: %s\n", shm_ptr);
        sem_post(&shm_mutex);
        sleep(1);
    }
    return NULL;
}

void shm_demo() {
    char initial_msg[100];
    
    printf("\n=== IPC SHARED MEMORY MODULE ===\n");
    printf("Demonstrates: fork(), shared memory (shmget/shmat), threads, semaphores\n\n");
    
    printf("Enter initial message to store in shared memory: ");
    getchar(); // Clear newline from previous input
    fgets(initial_msg, sizeof(initial_msg), stdin);
    initial_msg[strcspn(initial_msg, "\n")] = 0; // Remove newline
    
    printf("Enter number of messages for parent thread to write: ");
    scanf("%d", &num_messages);
    if(num_messages < 1) {
        printf("Invalid input. Please enter a positive value.\n");
        return;
    }
    
    printf("Enter number of reads for child process: ");
    scanf("%d", &num_reads);
    if(num_reads < 1) {
        printf("Invalid input. Please enter a positive value.\n");
        return;
    }
    
    printf("\nCreating shared memory segment...\n");
    key_t key = ftok("shm.c", 75);
    int shmid = shmget(key, SHM_SIZE, 0666|IPC_CREAT);
    if(shmid < 0) { perror("shmget"); return; }
    
    char* shm_ptr = (char*)shmat(shmid, NULL, 0);
    strcpy(shm_ptr, initial_msg);
    printf("Shared memory created. Initial message: %s\n\n", shm_ptr);

    sem_init(&shm_mutex, 0, 1);

    printf("Forking child process...\n");
    pid_t pid = fork();
    if(pid < 0) {
        perror("fork");
        return;
    } else if(pid == 0) {
        // Child process: read from shared memory
        printf("[Child Process] PID: %d started\n", getpid());
        for(int i = 0; i < num_reads; i++) {
            sem_wait(&shm_mutex);
            printf("[Child Process] Read from shared memory: %s\n", shm_ptr);
            sem_post(&shm_mutex);
            sleep(1);
        }
        shmdt(shm_ptr);
        printf("[Child Process] Detached from shared memory. Exiting.\n");
        exit(0);
    } else {
        // Parent process: create thread to write
        printf("[Parent Process] PID: %d, Child PID: %d\n", getpid(), pid);
        printf("[Parent Process] Creating writer thread...\n\n");
        
        pthread_t tid;
        pthread_create(&tid, NULL, thread_writer, shm_ptr);
        pthread_join(tid, NULL);
        
        wait(NULL);
        printf("\n[Parent Process] Child finished. Cleaning up shared memory.\n");
        shmdt(shm_ptr);
        shmctl(shmid, IPC_RMID, NULL);
        sem_destroy(&shm_mutex);
        printf("[Parent Process] Shared memory removed. Operation complete.\n");
    }
}
