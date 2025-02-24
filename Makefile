all: minheap

OBJS = heap.o 02-minheap.o
CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -lm

minheap: $(OBJS)
	$(CC) -o minheap $(CFLAGS) $(OBJS) $(LDFLAGS)

heap.o: heap.c heap.h
	$(CC) $(CFLAGS) -c heap.c -o heap.o

02-minheap.o: 02-minheap.c heap.h
	$(CC) $(CFLAGS) -c 02-minheap.c -o 01-minheap.o

clean:
	rm -f minheap $(OBJS)
