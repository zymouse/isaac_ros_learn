


CC=clang
CFLAGS=-g -Wall
OBJS=shard_memory.o
EXE=destroy_shmem.elf writeshmem.elf readshmem.elf


all: $(EXE)


%.elf: %.o $(OBJS)
	$(CC) $(CFLAGS) $< $(OBJS) -o $@

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.dSYM *.o $(EXE)