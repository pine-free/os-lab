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
  SERVER_PRINT("stopping server");
  DBG_PRINT("deleting semaphore set by key %d", SEMID);

  OR_DIE(semctl(SEMID, 0, IPC_RMID));

  DBG_PRINT("marking mem segment %d for destruction", SHMID);
  OR_DIE(shmctl(SHMID, IPC_RMID, NULL));

  exit(EXIT_SUCCESS);
}


int main() {
  set_debug();
  SERVER_PRINT("starting server");

  // Setup signals handling
  struct sigaction sig = {&server_teardown};
  sigaction(SIGINT, &sig, NULL);

  OR_DIE(SEMID = semget(SEM_KEY, 1, IPC_CREAT | 0600));

  DBG_PRINT("created semaphore set %d", SEMID);

  OR_DIE(semctl(SEMID, 0, SETVAL, 1));
  DBG_PRINT("intialized semaphore value to 1");

  int shmid;
  OR_DIE(SHMID = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0600));
  DBG_PRINT("created shared memory of size %zu with id %d", SHM_SIZE, SHMID);

  // Attach shared memory to addr space
  char* addr;
  OR_DIE(addr = shmat(SHMID, NULL, SHM_RDONLY));

  // wait until semaphore becomes 0
  struct sembuf sem_wait = {0, 0, 0};

  while(1) {
    DBG_PRINT("waiting for semaphore value to be set to 0");
    OR_DIE(semop(SEMID, &sem_wait, 1));

    SERVER_PRINT("got info: '%s'", addr);
  }

  return 0;
}
