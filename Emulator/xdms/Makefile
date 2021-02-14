SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

CC = gcc

.PHONY: all clean

all: $(OBJ)

clean:
	@echo "Cleaning up $(CURDIR)"
	@rm -f *.o

%.o: %.c $(DEPS)
	@echo "Compiling $<"
	@$(CC) $(MYFLAGS) -c -o $@ $<
