CC = gcc
CFLAGS = -Wall -g

all: format createfile readfile

format: format.o
	$(CC) $(CFLAGS) -o format format.o

createfile: createfile.o
	$(CC) $(CFLAGS) -o createfile createfile.o

readfile: readfile.o
	$(CC) $(CFLAGS) -o readfile readfile.o

format.o: format.c
	$(CC) $(CFLAGS) -c format.c

createfile.o: createfile.c
	$(CC) $(CFLAGS) -c createfile.c

readfile.o: readfile.c
	$(CC) $(CFLAGS) -c readfile.c

clean:
	rm -f *.o format createfile readfile