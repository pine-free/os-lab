#include "stdio.h"
#include "stdlib.h"
#include "sys/wait.h"
#include "unistd.h"
#include "variables.h"

void die(const char *msg) {
  fprintf(stderr, "%s\n", msg);
  exit(EXIT_FAILURE);
}


int run_subprocess(const char *path, char *const args[], char* wbuf, int wsize) {
  int child_to_parent[2];
  int parent_to_child[2];
  pid_t pid;

  if (pipe(child_to_parent) == -1) {
    die("pipe child to parent");
  }

  if (pipe(parent_to_child) == -1) {
    die("pipe parent to child");
  }

  if ((pid = fork()) == -1) {
    die("fork");
  }

  if (pid == 0) {
    dup2(child_to_parent[1], STDOUT_FILENO);
    close(child_to_parent[0]);
    close(child_to_parent[1]);
    execv(path, args);
    die("execl");
  } else {
    close(child_to_parent[1]);
    int nbytes = read(child_to_parent[0], wbuf, wsize);
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
