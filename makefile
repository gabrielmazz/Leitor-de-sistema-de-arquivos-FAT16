all: main

main: main.c
	gcc -o main main.c extra.c 

run: main
	./main

clean:
	rm -f main
