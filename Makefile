CC=LANG=C gcc
CFLAGS=-Wall -fstack-protector

SRCS := $(wildcard *.c)

build: $(SRCS)
	$(CC) $(SRCS) -o loc $(CFLAGS)
