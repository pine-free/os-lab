#include "stdio.h"
#include "stdlib.h"
#include "sys/wait.h"
#include "unistd.h"
#include "variables.h"

void die(const char *msg) {
  fprintf(stderr, "%s\n", msg);
  exit(EXIT_FAILURE);
}

void setup_subprocess(int *link, pid_t *pid) {
  if (pipe(link) == -1) {
    die("pipe");
  }

  if ((*pid = fork()) == -1) {
    die("fork");
  }
}

int run_subprocess(const char *path, char *const args[], char* wbuf, int size) {
  int link[2];
  pid_t pid;

  setup_subprocess(link, &pid);

  if (pid == 0) {
    dup2(link[1], STDOUT_FILENO);
    close(link[0]);
    close(link[1]);
    execv(path, args);
    die("execl");
  } else {
    close(link[1]);
    int nbytes = read(link[0], wbuf, size);
    wait(NULL);
    return nbytes;
  }

  return 1;
}

int main() {

  const char *path = "/usr/bin/find";
  char *const args[] = {"find", ".", "-name", "*.sh", NULL};
  char buf[1024];
  int res = run_subprocess(path, args, buf, sizeof(buf));
  printf("got (%.*s)\n", res, buf);
}
