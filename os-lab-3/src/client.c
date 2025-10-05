#include "errno.h"
#include "stdio.h"
#include "stdlib.h"
#include "sys/wait.h"
#include "unistd.h"
#include "variables.h"
#include <string.h>

int get_lines(char *buf, char *lines_buf[]) {
  char *sep = strtok(buf, "\n");
  int i = 0;
  while (sep != NULL) {
    char *arg = malloc(128);
    strcpy(arg, sep);
    lines_buf[i++] = arg;
    sep = strtok(NULL, "\n");
  }

  return i;
}

void die(const char *msg) {
  fprintf(stderr, "%s\n", msg);
  fprintf(stderr, "ERRNO: %d\n", errno);
  exit(EXIT_FAILURE);
}

int run_subprocess(const char *path, char *const args[], const char *rbuf,
                   int rsize, char *wbuf, int wsize) {
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

int run_subprocess_nopipe(const char *path, char *const args[], char *wbuf,
                          int wsize) {
  return run_subprocess(path, args, "", 0, wbuf, wsize);
}

int run_find(char *buf, int size) {
  const char *find_path = "/usr/bin/find";
  char *const find_args[] = {"find", ".", "-name", "*.sh", NULL};

  int res = run_subprocess_nopipe(find_path, find_args, buf, size);
  return res;
}

int run_wc(char *rbuf, int rsize, char *wbuf, int wsize) {
  const char *wc_path = "/usr/bin/wc";
  char *files[100] = {};
  int nfiles = get_lines(rbuf, files);
  char *wc_args[100] = {"wc", "-l"};
  int i = 2;

  for (int j = 0; j < nfiles; ++j) {
    wc_args[i++] = files[j];
  }

  wc_args[i] = NULL;

  int res = run_subprocess_nopipe(wc_path, wc_args, wbuf, wsize);

  return res;
}

int run_tail(const char *rbuf, int rsize, char *wbuf, int wsize) {
  const char *tail_path = "/usr/bin/tail";
  char *tail_args[] = {"tail", "-n", "1", NULL};

  int res = run_subprocess(tail_path, tail_args, rbuf, rsize, wbuf, wsize);
  return res;
}

int run_du(const char *rbuf, int rsize, char *wbuf, int wsize) {}

int main() {

  const char *wc_path = "/usr/bin/wc";
  const char *tail = "/usr/bin/tail";
  char buf[1024];

  // Run find to get the files
  int res = run_find(buf, sizeof(buf));
  printf("got (%.*s)\n", res, buf);

  char files_buf[1024];
  strcpy(files_buf, buf);

  // run wc to get the line count
  res = run_wc(files_buf, strlen(files_buf), buf, sizeof(buf));

  printf("got (%.*s)\n", res, buf);

  // run wc to get the line count
  res = run_tail(buf, strlen(buf), buf, sizeof(buf));

  printf("got (%.*s)\n", res, buf);
  printf("lines: %d\n", atoi(buf));
}
