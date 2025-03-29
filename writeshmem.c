#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shard_memory.h"

int main(int argc, char const *argv[])
{
  if (argc != 2) {
    printf("usage - %s [stuff to write]", argv[0]);
    return -1;
  }

  //grab ths memory block
  char *shmem_ptr = attach_memory_block(FILENAME, BLOCK_SIZE);
  if (shmem_ptr == NULL){
    printf("ERRROR: couldn't get mmap shard_memory\n" );
    return -1;
  }

  printf("Writing: \"%s\"\n", argv[1]);
  strncpy(shmem_ptr, argv[1], BLOCK_SIZE);

  detach_memory_block(shmem_ptr);

  return 0;
}
