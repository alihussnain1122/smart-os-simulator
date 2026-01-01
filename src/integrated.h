// src/integrated.h
#ifndef INTEGRATED_H
#define INTEGRATED_H

#include "cpu/cpu.h"
#include "memory/memory.h"
#include "paging/paging.h"
#include "deadlock/banker.h"

// Colors
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN    "\033[1;36m"
#define RESET   "\033[0m"

// Integrated process structure containing all data for a process
typedef struct {
    int pid;
    
    // CPU scheduling data
    int arrival_time;
    int burst_time;
    int remaining_time;
    int waiting_time;
    int turnaround_time;
    int completed;
    
    // Memory management data
    int memory_required;
    int allocated_block;
    int memory_allocated;
    
    // Paging data
    int *page_references;
    int num_pages;
    int page_faults;
    
    // Deadlock/Resource data
    int *allocation;
    int *max_need;
    int *current_need;
    int num_resources;
} IntegratedProcess;

// Function declarations
void integrated_flow_mode();
void print_integrated_report(IntegratedProcess processes[], int n);

#endif
