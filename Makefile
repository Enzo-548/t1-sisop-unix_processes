CC=gcc
CFLAGS=-Wall -O2 -pthread

SRC=main.c \
    threads/threads.c \
    processes/processes.c \
    ipc/shm.c \
    ipc/semaphore.c \
    common/timer.c

OUT=counter

all: run

build:
	$(CC) $(SRC) -o $(OUT) $(CFLAGS)

run: build
	./$(OUT)

clean:
	rm -f $(OUT) results.csv
