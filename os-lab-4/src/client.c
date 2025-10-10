#include "common.h"
#include "variables.h"
#include <sys/sem.h>
#include <sys/shm.h>

#define CLIENT_PRINT(...) INFO_PRINT("client", __VA_ARGS__)

static int SEMID;
static int SHMID;

int main() {
  set_debug();
  CLIENT_PRINT("starting client");

  OR_DIE(SEMID = semget(SEM_KEY, 0, 0));
  DBG_PRINT("got semaphore set %d", SEMID);

  OR_DIE(SHMID = shmget(SHM_KEY, SHM_SIZE, 0));
  DBG_PRINT("got share memory %d", SHMID);

  char* addr;
  OR_DIE(addr = shmat(SHMID, NULL, 0));
  DBG_PRINT("attached shared memory from %d", SHMID);


  return 0;
}
