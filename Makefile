CC=gcc
CFLAGS=-L. -l:files.a

build:
	$(CC) -c files.c -o files.o
	ar rcs files.a files.o
	$(CC) main.c -o loc $(CFLAGS)
