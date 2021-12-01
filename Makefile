all: shell.o
	gcc -o cshell shell.o

shell.o: shell.c shell.h
	gcc -c shell.c
	
run:
	./cshell
