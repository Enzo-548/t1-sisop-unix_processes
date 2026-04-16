all: compile

CFLAGS ?= -O2 -Wall -Wextra -std=c11
LDFLAGS ?= -pthread

compile:
	@gcc $(CFLAGS) main.c -o main $(LDFLAGS)

run: compile
	@./main
