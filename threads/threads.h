// threads/threads.h
// Header for thread demonstrations including mutex and semaphore synchronization
#ifndef THREADS_H
#define THREADS_H

#include <pthread.h>
#include <semaphore.h>

// Thread demonstration functions
void threads_demo();
void mutex_demo();
void semaphore_demo();
void producer_consumer_demo();

#endif
