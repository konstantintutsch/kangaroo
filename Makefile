CC=gcc
CFLAGS=

SRCS := $(wildcard *.c)

build: $(SRCS)
	$(CC) $(SRCS) -o loc $(CFLAGS)
