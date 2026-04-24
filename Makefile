all: run

CFLAGS = -Wall -Wextra
PFLAGS = -lpthread -lrt

PROG = n_main

compile:
	@gcc $(CFLAGS) $(PROG).c -o $(PROG) $(PFLAGS)

run: compile
	@./$(PROG)

clean:
	@rm -f $(PROG) %.o
