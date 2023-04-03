CC = gcc
CFLAGS= -Wall -Wextra -O3 -march=native -fstack-protector-strong
LDFLAGS= -lm
EXEC = main

all: $(EXEC)

$(EXEC): main.o extra.o
	$(CC) -o $@ $^ $(LDFLAGS)

main.o: main.c extra.h
	$(CC) -c $< $(CFLAGS)

extra.o: extra.c extra.h
	$(CC) -c $< $(CFLAGS)

run: $(EXEC)
	./$(EXEC)

clean:
	rm -rf *.o $(EXEC)
