#include "stdio.h"
#include "stdlib.h"
#include "sys/wait.h"
#include "unistd.h"
#include "variables.h"


void die(const char* msg) {
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

int ls_sh() {
  int link[2];
  pid_t pid;
  char buf[1024];

  const char* path = "/usr/bin/find";
  char *const args[] = {"find", ".",  "-name", "*.sh", NULL};

  setup_subprocess(link, &pid);

  if (pid == 0) {
    dup2(link[1], STDOUT_FILENO);
    close(link[0]);
    close(link[1]);
    execv(path, args);
    die("execl");
  } else {
    close(link[1]);
    int nbytes = read(link[0], buf, sizeof(buf));
    printf("got (%.*s)\n", nbytes, buf);
    wait(NULL);
  }

  return 1;
}

int main() { ls_sh(); }
