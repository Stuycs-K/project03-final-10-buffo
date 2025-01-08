all: client server

client: client.o pipe_networking.o
	gcc -o client client.o pipe_networking.o

server: server.o pipe_networking.o
	gcc -o server server.o pipe_networking.o

client.o: client.c client.h pipe_networking.h
	gcc -c client.c

server.o: server.c server.h pipe_networking.h
	gcc -c server.c

pipe_networking.o: pipe_networking.c pipe_networking.h
	gcc -c pipe_networking.c

clean:
	rm *.o
	rm *~
