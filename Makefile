CC=LANG=C gcc
CFLAGS=-Wall -fstack-protector -lzarg

SRCS := $(wildcard *.c)

build: $(SRCS)
	$(CC) $(SRCS) -o kangaroo $(CFLAGS)
