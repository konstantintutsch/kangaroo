CC=gcc
CFLAGS=-L. -l:files.a -l:parse.a

build:
	$(CC) -c files.c -o files.o
	ar rcs files.a files.o
	$(CC) -c parse.c -o parse.o
	ar rcs parse.a parse.o
	$(CC) main.c -o loc $(CFLAGS)
