
CC = g++

CFLAGS = -std=c++17 -Wall -Wextra

all: client server

client: client.o
	$(CC) $(CFLAGS) -o client client.o -lrt -lpthread

client.o: client.cpp
	$(CC) $(CFLAGS) -c client.cpp

server: server.o
	$(CC) $(CFLAGS) -o server server.o -lrt -lpthread

server.o: server.cpp
	$(CC) $(CFLAGS) -c server.cpp

clean:
	rm -f client server *.o
