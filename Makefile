#Something Something Setup
#Ordenar de baixo pra cima
all: run

compile:
	@gcc main.c -o main

run: compile
	@./main
