CC = gcc
CFLAGS = -Iinclude -Wall -Wextra
SRC = src/main.c
OBJ = $(SRC:.c=.o)
EXEC = tatooine

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)

run: $(EXEC)
	./$(EXEC)

.PHONY: all clean run