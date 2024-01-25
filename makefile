CC = g++
CFLAGS = -std=c++11 -Wall -stdlib=libc++

all: main

main: main.o server.o client.o manage.o accountsCredit.o
	$(CC) $(CFLAGS) -o main main.o server.o client.o manage.o accountsCredit.o

main.o: main.cpp server.h client.h manage.h accounts.h accountsCredit.h
	$(CC) $(CFLAGS) -c main.cpp

server.o: server.cpp server.h client.h manage.h accounts.h accountsCredit.h
	$(CC) $(CFLAGS) -c server.cpp

client.o: client.cpp client.h
	$(CC) $(CFLAGS) -c client.cpp

manage.o: manage.cpp manage.h accounts.h accountsCredit.h
	$(CC) $(CFLAGS) -c manage.cpp

accountsCredit.o: accountsCredit.cpp accountsCredit.h
	$(CC) $(CFLAGS) -c accountsCredit.cpp

clean:
	rm -f main *.o
