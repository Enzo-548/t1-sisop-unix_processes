all: compile

CFLAGS ?= -O2 -Wall -Wextra -std=c11
LDFLAGS ?= -pthread

MODE ?= T2
N ?= 4
TOTAL ?= 10000000

compile:
	@gcc $(CFLAGS) main.c -o main $(LDFLAGS)

run: compile
	@./main $(MODE) $(N) $(TOTAL)
