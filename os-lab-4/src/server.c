#include "common.h"
#include "debug.h"
#include "variables.h"
#include "sys/sem.h"
#include "sys/shm.h"
#include <signal.h>
#include <stdlib.h>
#include <sys/ipc.h>

#define SERVER_PRINT(...) INFO_PRINT("server", __VA_ARGS__)

static int SEMID;
static int SHMID;

void server_teardown(int signal) {
  if (DEBUG) {
    DBG_PRINT("deleting semaphore set by key %d", SEMID);
  }

  OR_DIE(semctl(SEMID, 0, IPC_RMID));

  if (DEBUG) {
    DBG_PRINT("marking mem segment %d for destruction", SHMID);
  }
  OR_DIE(shmctl(SHMID, IPC_RMID, NULL));

  exit(EXIT_SUCCESS);
}


int main() {
  set_debug();
  SERVER_PRINT("starting server");

  // Setup signals handling
  struct sigaction sig = {&server_teardown};
  sigaction(SIGINT, &sig, NULL);

  OR_DIE(SEMID = semget(SEM_KEY, 1, IPC_CREAT));

  if (DEBUG) {
    DBG_PRINT("created semaphore set %d", SEMID);
  }

  int shmid;
  OR_DIE(SHMID = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT));

  if (DEBUG) {
    DBG_PRINT("created shared memory of size %zu with id %d", SHM_SIZE, SHMID);
  }

  while(1) {
    
  }
  

  return 0;
}
