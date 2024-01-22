CC = g++
CFLAGS = -std=c++11 -Wall -stdlib=libc++

all: main

main: main.o server.o client.o
	$(CC) $(CFLAGS) -o main main.o server.o client.o

main.o: main.cpp server.h client.h
	$(CC) $(CFLAGS) -c main.cpp

server.o: server.cpp server.h client.h
	$(CC) $(CFLAGS) -c server.cpp

client.o: client.cpp client.h
	$(CC) $(CFLAGS) -c client.cpp

clean:
	rm -f main *.o
