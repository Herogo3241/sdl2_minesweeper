# Makefile

build: 
	gcc -Wall -std=c99 main.c -lSDL2 -lSDL2_ttf -o main

run: build
	./main

clean:
	rm -f main



