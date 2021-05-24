CC = gcc

all:	main.o 
	$(CC) -o Proj1.exe main.o
	
main.o:	src/main.c inc/main.h 
	$(CC) -Iinc -O0 -g3 -Wall -c src/main.c
	$(CC) -Iinc -masm=intel -g3 -Wall -c src/main.c -S
	
clean:
	del -rf *.s *.o *.exe