#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shard_memory.h"

#define IPC_RESULT_ERROR (-1)

static int get_shared_block(char *filename, int size)
{
  key_t key;

  // Request a key
  // The key is linked to a filename, so that other programs can access it.
  key = ftok(FILENAME, 0);
  if(key == IPC_RESULT_ERROR)
    return IPC_RESULT_ERROR;

  // get shared block --- create it if doesn't exist
  return shmget(key, size, 0644 | IPC_CREAT);
}

char * attach_memory_block(char *filename, int size)
{
  int shard_block_id = get_shared_block(filename, size);
  char *result;
  if (shard_block_id == IPC_RESULT_ERROR){
    printf("ERRROR: 无法获取block id\n" );
    return NULL;
  }
  
  // map the shard block into this process's memory
  // and give me a pointer to it 
  result = shmat(shard_block_id, NULL, 0);
  if (result == (char*)IPC_RESULT_ERROR){
    printf("ERRROR: 无法获取映射地址 id\n" );
    return NULL;
  }
  
  return result;
}

bool detach_memory_block(char *block)
{
  return (shmdt(block) != IPC_RESULT_ERROR);
}

bool destroy_memory_block(char *filename)
{
  int shard_block_id = get_shared_block(filename, 0);
  
  if (shard_block_id == IPC_RESULT_ERROR)
    return NULL;

  return (shmctl(shard_block_id, IPC_RMID, NULL) != IPC_RESULT_ERROR);
}
