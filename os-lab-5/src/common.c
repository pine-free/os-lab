#include "common.h"

int DEBUG = 0;

int get_lines(char *buf, char *lines_buf[]) {
  char *sep = strtok(buf, "\n");
  int i = 0;
  while (sep != NULL) {
    char *arg = malloc(128);
    strcpy(arg, sep);
    if (DEBUG) {
      DBG_PRINT("got line '%s'\n", arg);
    }
    lines_buf[i++] = arg;
    sep = strtok(NULL, "\n");
  }

  return i;
}

void append_arr(int start, char *dst[], char *src[], int src_size) {
  for (int j = 0; j < src_size; ++j) {
    dst[start + j] = src[j];
  }
}

void print_err(const char *msg) {
  ERR_PRINT("%s", msg);
  ERR_PRINT("ERRNO: %d", errno);
}

void die(const char *msg) {
  print_err(msg);
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

    OR_FAIL(execv(path, args));
    die("execv");
  } else {
    close(child_to_parent[1]);
    close(parent_to_child[0]);

    OR_FAIL(write(parent_to_child[1], rbuf, rsize));
    close(parent_to_child[1]);

    wait(NULL);
    int nbytes = read(child_to_parent[0], wbuf, wsize);
    return nbytes;
  }

  return 1;
}

int run(const char *path, char *const args[], char *wbuf, int wsize) {
  int res = run_subprocess(path, args, "", 0, wbuf, wsize);
  wbuf[--res] = 0;

  if (DEBUG) {
    DBG_PRINT("%s output: '%.*s'", path, res, wbuf);
  }

  return res;
}

int rpipe(const char *path, char *const args[], char *rbuf, int rsize,
          char *wbuf, int wsize) {
  int res = run_subprocess(path, args, rbuf, rsize, wbuf, wsize);
  wbuf[--res] = 0;

  if (DEBUG) {
    DBG_PRINT("%s output: '%.*s'", path, res, wbuf);
  }

  return res;
}
