#include "stdio.h"
#include "stdlib.h"
#include "sys/wait.h"
#include "unistd.h"
#include "variables.h"
#include <string.h>
#include "errno.h"


void die(const char *msg) {
  fprintf(stderr, "%s\n", msg);
  fprintf(stderr, "ERRNO: %d\n", errno);
  exit(EXIT_FAILURE);
}

int run_subprocess(const char *path, char *const args[], char* rbuf, int rsize, char* wbuf, int wsize) {
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
    close(child_to_parent[1]);

    close(parent_to_child[1]);
    dup2(parent_to_child[0], STDIN_FILENO);
    close(parent_to_child[0]);

    execv(path, args);
    die("execv");
  } else {
    close(child_to_parent[1]);
    close(parent_to_child[0]);

    if (write(parent_to_child[1], rbuf, rsize) == -1) {
      die("write to child stdin");
    }
    close(parent_to_child[1]);


    wait(NULL);
    int nbytes = read(child_to_parent[0], wbuf, wsize);
    return nbytes;
  }

  return 1;
}

int run_subprocess_nopipe(const char *path, char *const args[], char* wbuf, int wsize) {
  return run_subprocess(path, args, "", 0, wbuf, wsize);
}


int run_find(char* buf, int size) {
  const char *find_path = "/usr/bin/find";
  char *const find_args[] = {"find", ".", "-name", "*.sh", NULL};

  int res = run_subprocess_nopipe(find_path, find_args, buf, size);
  return res;
}

int run_wc(char* buf, int size) {
  const char *wc_path= "/usr/bin/wc";
  char* wc_args[100] = {"wc", "-l"};
  int i = 2;
  char *sep = strtok(buf, "\n");
  while (sep != NULL) {
    char* arg = malloc(50);
    strcpy(arg, sep);
    wc_args[i++] = arg;
    sep = strtok(NULL, "\n");
  }

  wc_args[i] = NULL;

  int res = run_subprocess_nopipe(wc_path, wc_args, buf, size);

  return res;
}

int main() {

  const char *wc_path= "/usr/bin/wc";
  const char *tail= "/usr/bin/tail";
  char buf[1024];

  // Run find to get the files
  int res = run_find(buf, sizeof(buf));
  printf("got (%.*s)\n", res, buf);

  res = run_wc(buf, sizeof(buf));

  // run wc to get the line count


  
  printf("got (%.*s)\n", res, buf);
}
