#include "common.h"
#include "variables.h"
#include <sys/sem.h>
#include <sys/shm.h>

#define CLIENT_PRINT(...) INFO_PRINT("client", __VA_ARGS__)

static int SEMID;
static int SHMID;

int main() {
  set_debug();
  CLIENT_PRINT("running client with pid %d", getpid());

  OR_DIE(SEMID = semget(SEM_KEY, 0, 0));
  DBG_PRINT("got semaphore set %d", SEMID);

  OR_DIE(SHMID = shmget(SHM_KEY, SHM_SIZE, 0));
  DBG_PRINT("got shared memory %d", SHMID);

  char *addr;
  OR_DIE(addr = shmat(SHMID, NULL, 0));
  DBG_PRINT("attached shared memory from %d", SHMID);
  
  DBG_PRINT("running cal and putting output to shared mem");
  RUN("/usr/bin/cal", addr, SHM_SIZE, "cal", "-v", "--color=never");

  DBG_PRINT("detaching from shared memory");
  OR_DIE(shmdt(addr));

  DBG_PRINT("decrementing semaphore");
  struct sembuf sem_dec = {0, -1, 0};
  OR_DIE(semop(SEMID, &sem_dec, 1));

  return 0;
}
