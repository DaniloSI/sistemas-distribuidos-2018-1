all: server.o client.o
	gcc server.o -o server.exe
	gcc client.o -o client.exe
	make clean

server.o: server.c
	gcc -c server.c

client.o: client.c
	gcc -c client.c

calculadora.o: calculadora.c calculadora.h
	gcc -c calculadora.c

clean:
	rm *.o
