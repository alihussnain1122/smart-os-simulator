// threads/threads.c
// Demonstrates threads, mutexes, and semaphores for OS synchronization concepts
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "threads.h"

#define MAX_THREADS 20
#define MAX_BUFFER 10

// ==================== MUTEX ====================
pthread_mutex_t mutex;
int shared_counter = 0;
int mutex_iterations = 2;

void* mutex_thread_func(void* arg) {
    int thread_id = *(int*)arg;
    
    for(int i = 0; i < mutex_iterations; i++) {
        pthread_mutex_lock(&mutex);  // Lock mutex before accessing shared resource
        
        int temp = shared_counter;
        printf("[Thread %d] Read counter: %d\n", thread_id, temp);
        temp++;
        sleep(1);  // Simulate some work
        shared_counter = temp;
        printf("[Thread %d] Updated counter to: %d\n", thread_id, shared_counter);
        
        pthread_mutex_unlock(&mutex);  // Unlock mutex after accessing shared resource
        sleep(1);
    }
    
    pthread_exit(NULL);
}

void mutex_demo() {
    int num_threads;
    
    printf("\n=== MUTEX (pthread_mutex_lock/unlock) ===\n");
    printf("Multiple threads will increment a shared counter safely using mutex\n\n");
    
    printf("Enter number of threads (1-%d): ", MAX_THREADS);
    scanf("%d", &num_threads);
    if(num_threads < 1 || num_threads > MAX_THREADS) {
        printf("Invalid input. Please enter a value between 1 and %d.\n", MAX_THREADS);
        return;
    }
    
    printf("Enter number of iterations per thread: ");
    scanf("%d", &mutex_iterations);
    if(mutex_iterations < 1) {
        printf("Invalid input. Please enter a positive value.\n");
        return;
    }
    
    pthread_t threads[num_threads];
    int thread_ids[num_threads];
    
    // Initialize mutex
    pthread_mutex_init(&mutex, NULL);
    shared_counter = 0;
    
    printf("\nStarting %d threads with %d iterations each...\n\n", num_threads, mutex_iterations);
    
    // Create threads
    for(int i = 0; i < num_threads; i++) {
        thread_ids[i] = i + 1;
        pthread_create(&threads[i], NULL, mutex_thread_func, &thread_ids[i]);
        printf("Created Thread %d\n", i + 1);
    }
    
    // Wait for all threads to complete
    for(int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
        printf("Thread %d completed\n", i + 1);
    }
    
    printf("\nFinal counter value: %d (Expected: %d)\n", shared_counter, num_threads * mutex_iterations);
    
    // Destroy mutex
    pthread_mutex_destroy(&mutex);
    printf("Mutex destroyed. Operation complete.\n");
}

// ==================== SEMAPHORE ====================
sem_t semaphore;
int semaphore_shared_resource = 0;
int sem_work_time = 2;

void* semaphore_thread_func(void* arg) {
    int thread_id = *(int*)arg;
    
    printf("[Thread %d] Waiting on semaphore...\n", thread_id);
    sem_wait(&semaphore);  // Wait (decrement semaphore)
    
    printf("[Thread %d] Acquired semaphore! Accessing shared resource.\n", thread_id);
    semaphore_shared_resource = thread_id;
    printf("[Thread %d] Set shared resource to %d\n", thread_id, semaphore_shared_resource);
    
    sleep(sem_work_time);  // Simulate critical section work
    
    printf("[Thread %d] Releasing semaphore.\n", thread_id);
    sem_post(&semaphore);  // Signal (increment semaphore)
    
    pthread_exit(NULL);
}

void semaphore_demo() {
    int num_threads, sem_value;
    
    printf("\n=== SEMAPHORE (sem_wait/sem_post) ===\n");
    printf("Semaphore controlling access to a shared resource\n\n");
    
    printf("Enter number of threads (1-%d): ", MAX_THREADS);
    scanf("%d", &num_threads);
    if(num_threads < 1 || num_threads > MAX_THREADS) {
        printf("Invalid input. Please enter a value between 1 and %d.\n", MAX_THREADS);
        return;
    }
    
    printf("Enter initial semaphore value (1 for binary/mutex, >1 for counting): ");
    scanf("%d", &sem_value);
    if(sem_value < 1) {
        printf("Invalid input. Please enter a positive value.\n");
        return;
    }
    
    printf("Enter work time in critical section (seconds): ");
    scanf("%d", &sem_work_time);
    if(sem_work_time < 1) sem_work_time = 1;
    
    pthread_t threads[num_threads];
    int thread_ids[num_threads];
    
    // Initialize semaphore
    sem_init(&semaphore, 0, sem_value);
    
    printf("\nStarting %d threads with semaphore value %d...\n\n", num_threads, sem_value);
    
    // Create threads
    for(int i = 0; i < num_threads; i++) {
        thread_ids[i] = i + 1;
        pthread_create(&threads[i], NULL, semaphore_thread_func, &thread_ids[i]);
    }
    
    // Wait for all threads to complete
    for(int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("\nSemaphore operation complete.\n");
    
    // Destroy semaphore
    sem_destroy(&semaphore);
}

// ==================== PRODUCER-CONSUMER (BOUNDED BUFFER) ====================
int *pc_buffer;
int pc_buffer_size = 5;
int pc_count = 0;
int pc_in = 0;
int pc_out = 0;
int items_per_producer = 3;
int items_per_consumer = 3;

pthread_mutex_t pc_mutex;
sem_t empty_slots;  // Counts empty slots
sem_t full_slots;   // Counts filled slots

void* producer_func(void* arg) {
    int producer_id = *(int*)arg;
    
    for(int i = 0; i < items_per_producer; i++) {
        int item = (producer_id * 100) + i + 1;
        
        sem_wait(&empty_slots);  // Wait for an empty slot
        pthread_mutex_lock(&pc_mutex);  // Lock buffer access
        
        pc_buffer[pc_in] = item;
        printf("[Producer %d] Produced item %d at index %d\n", producer_id, item, pc_in);
        pc_in = (pc_in + 1) % pc_buffer_size;
        pc_count++;
        
        pthread_mutex_unlock(&pc_mutex);  // Unlock buffer access
        sem_post(&full_slots);  // Signal that a slot is now full
        
        sleep(1);
    }
    
    pthread_exit(NULL);
}

void* consumer_func(void* arg) {
    int consumer_id = *(int*)arg;
    
    for(int i = 0; i < items_per_consumer; i++) {
        sem_wait(&full_slots);  // Wait for a filled slot
        pthread_mutex_lock(&pc_mutex);  // Lock buffer access
        
        int item = pc_buffer[pc_out];
        printf("[Consumer %d] Consumed item %d from index %d\n", consumer_id, item, pc_out);
        pc_out = (pc_out + 1) % pc_buffer_size;
        pc_count--;
        
        pthread_mutex_unlock(&pc_mutex);  // Unlock buffer access
        sem_post(&empty_slots);  // Signal that a slot is now empty
        
        sleep(1);
    }
    
    pthread_exit(NULL);
}

void producer_consumer_demo() {
    int num_producers, num_consumers;
    
    printf("\n=== PRODUCER-CONSUMER (BOUNDED BUFFER) ===\n");
    printf("Classic bounded buffer problem using mutex + semaphores\n\n");
    
    printf("Enter buffer size (1-%d): ", MAX_BUFFER);
    scanf("%d", &pc_buffer_size);
    if(pc_buffer_size < 1 || pc_buffer_size > MAX_BUFFER) {
        printf("Invalid input. Please enter a value between 1 and %d.\n", MAX_BUFFER);
        return;
    }
    
    printf("Enter number of producers (1-5): ");
    scanf("%d", &num_producers);
    if(num_producers < 1 || num_producers > 5) {
        printf("Invalid input. Please enter a value between 1 and 5.\n");
        return;
    }
    
    printf("Enter number of consumers (1-5): ");
    scanf("%d", &num_consumers);
    if(num_consumers < 1 || num_consumers > 5) {
        printf("Invalid input. Please enter a value between 1 and 5.\n");
        return;
    }
    
    printf("Enter items to produce per producer: ");
    scanf("%d", &items_per_producer);
    if(items_per_producer < 1) {
        printf("Invalid input. Please enter a positive value.\n");
        return;
    }
    
    printf("Enter items to consume per consumer: ");
    scanf("%d", &items_per_consumer);
    if(items_per_consumer < 1) {
        printf("Invalid input. Please enter a positive value.\n");
        return;
    }
    
    // Check balance
    int total_produced = num_producers * items_per_producer;
    int total_consumed = num_consumers * items_per_consumer;
    if(total_produced != total_consumed) {
        printf("\nWarning: Total produce (%d) != Total consume (%d)\n", total_produced, total_consumed);
        printf("Adjusting consumer items to balance...\n");
        items_per_consumer = total_produced / num_consumers;
        if(items_per_consumer < 1) items_per_consumer = 1;
    }
    
    // Allocate buffer
    pc_buffer = (int*)malloc(pc_buffer_size * sizeof(int));
    
    pthread_t producers[num_producers], consumers[num_consumers];
    int producer_ids[num_producers], consumer_ids[num_consumers];
    
    // Initialize synchronization primitives
    pthread_mutex_init(&pc_mutex, NULL);
    sem_init(&empty_slots, 0, pc_buffer_size);
    sem_init(&full_slots, 0, 0);
    
    // Reset buffer state
    pc_count = 0;
    pc_in = 0;
    pc_out = 0;
    
    printf("\nStarting %d producers and %d consumers with buffer size %d...\n\n", 
           num_producers, num_consumers, pc_buffer_size);
    
    // Create threads
    for(int i = 0; i < num_producers; i++) {
        producer_ids[i] = i + 1;
        pthread_create(&producers[i], NULL, producer_func, &producer_ids[i]);
    }
    for(int i = 0; i < num_consumers; i++) {
        consumer_ids[i] = i + 1;
        pthread_create(&consumers[i], NULL, consumer_func, &consumer_ids[i]);
    }
    
    // Wait for all threads
    for(int i = 0; i < num_producers; i++) pthread_join(producers[i], NULL);
    for(int i = 0; i < num_consumers; i++) pthread_join(consumers[i], NULL);
    
    printf("\nProducer-Consumer operation complete.\n");
    
    // Cleanup
    free(pc_buffer);
    pthread_mutex_destroy(&pc_mutex);
    sem_destroy(&empty_slots);
    sem_destroy(&full_slots);
}

// ==================== MAIN FUNCTION ====================
void threads_demo() {
    int choice;
    
    printf("\n========== THREADS & SYNCHRONIZATION MODULE ==========\n");
    printf("[1] Mutex (pthread_mutex_lock/unlock)\n");
    printf("[2] Semaphore (sem_wait/sem_post)\n");
    printf("[3] Producer-Consumer (Mutex + Semaphore)\n");
    printf("Select Option: ");
    scanf("%d", &choice);
    
    switch(choice) {
        case 1:
            mutex_demo();
            break;
        case 2:
            semaphore_demo();
            break;
        case 3:
            producer_consumer_demo();
            break;
        default:
            printf("Invalid choice!\n");
    }
}
