.PHONY: all run clean

CC = gcc
CFLAGS = -Wall -Wextra -Icontext_comparison

# Todos os arquivos fonte
SRC = main.c \
	context_comparison/stress_test.c \
	context_comparison/process_context/process_context.c \
	context_comparison/thread_context/thread_context.c

# Converte .c → .o
OBJ = $(SRC:.c=.o)

TARGET = main

# Fluxo principal
all: run

# Linkagem final
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

# Compilação de cada .c
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Executa
run: $(TARGET)
	./$(TARGET)

# Limpa build
clean:
	rm -f $(OBJ) $(TARGET)