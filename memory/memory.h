#ifndef MEMORY_H
#define MEMORY_H

typedef struct {
    int size;
    int allocated;
} Block;

typedef struct {
    int pid;
    int size;
    int blockIndex;
} MemProcess;

void first_fit(Block blocks[], int b, MemProcess p[], int n);
void best_fit(Block blocks[], int b, MemProcess p[], int n);
void worst_fit(Block blocks[], int b, MemProcess p[], int n);

#endif
