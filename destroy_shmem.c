#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shard_memory.h"

int main(int argc, char const *argv[])
{
  if (argc != 1) {
    printf("usage - %s  (no args)", argv[0]);
    return -1;
  }

  //grab ths memory block
  char *shmem_ptr = attach_memory_block(FILENAME, BLOCK_SIZE);
  if (shmem_ptr == NULL){
    printf("ERRROR: couldn't get mmap shard_memory\n" );
    return -1;
  }

  if (destroy_memory_block(FILENAME)) {
    printf("Destoryed block: %s\n", FILENAME);
  }else{
    printf("Cloud not destory blockj: %s\n", FILENAME);
  }

  return 0;

}
