CC      = gcc
CFLAGS  = -Wall -Wextra -g -I. -Isrc
LDFLAGS = -lpthread
TARGET  = ali

# Core source files
SRC_CORE = src/main.c src/integrated.c src/individual.c src/report.c


# Module source files
SRC_CPU      = cpu/fcfs.c cpu/sjf.c cpu/round_robin.c
SRC_MEMORY   = memory/first_fit.c memory/best_fit.c memory/worst_fit.c
SRC_PAGING   = paging/paging.c paging/fifo.c paging/lru.c
SRC_DEADLOCK = deadlock/banker.c
SRC_PROCESS  = process/process.c
SRC_THREADS  = threads/threads.c

# IPC source
SRC_IPC = ipc/shm.c

# All sources
SRC = $(SRC_CORE) $(SRC_CPU) $(SRC_MEMORY) $(SRC_PAGING) $(SRC_DEADLOCK) $(SRC_PROCESS) $(SRC_THREADS) $(SRC_IPC)

# Object files
OBJ = $(SRC:.c=.o)

# Default target
all: $(TARGET)

# Link executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile object files (works for all folders)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJ) $(TARGET)

# Run
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
