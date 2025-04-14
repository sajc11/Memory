CC = gcc
CFLAGS = -Wall

# Object files list
OBJS = main.o memory.o stats.o lru.o utils.o

all: vm

vm: $(OBJS)
	$(CC) $(CFLAGS) -o vm $(OBJS)

main.o: main.c memory.h stats.h utils.h
memory.o: memory.c memory.h lru.h stats.h
stats.o: stats.c stats.h
lru.o: lru.c lru.h
utils.o: utils.c utils.h

clean:
	rm -f *.o vm
