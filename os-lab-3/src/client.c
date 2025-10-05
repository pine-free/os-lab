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

void append_arr(int start, char* dst[], char* src[], int src_size) {
  for (int j = 0; j < src_size; ++j) {
    dst[start + j] = src[j];
  }
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

int run_wc(char **files, int nfiles, char *wbuf, int wsize) {
  const char *wc_path = "/usr/bin/wc";
  char *wc_args[100] = {"wc", "-l"};
  append_arr(2, wc_args, files, nfiles);
  wc_args[2 + nfiles] = NULL;

  int res = run_subprocess_nopipe(wc_path, wc_args, wbuf, wsize);

  return res;
}

int run_tail(const char *rbuf, int rsize, char *wbuf, int wsize) {
  const char *tail_path = "/usr/bin/tail";
  char *tail_args[] = {"tail", "-n", "1", NULL};

  int res = run_subprocess(tail_path, tail_args, rbuf, rsize, wbuf, wsize);
  return res;
}

int run_du(char **files, int nfiles, char *wbuf, int wsize) {
  const char* du_path = "/usr/bin/du";
  char* du_args[100] = {"du",  "-b"};
  append_arr(2, du_args, files, nfiles);
  du_args[2 + nfiles] = NULL;

  int res = run_subprocess_nopipe(du_path, du_args, wbuf, wsize);
  return res;
}

int main() {

  const char *wc_path = "/usr/bin/wc";
  const char *tail = "/usr/bin/tail";
  char buf[1024];

  // Run find to get the files
  int res = run_find(buf, sizeof(buf));

  char* files_buf[100];
  int nfiles = get_lines(buf, files_buf);

  // run wc to get the line count
  res = run_wc(files_buf, nfiles, buf, sizeof(buf));
  res = run_tail(buf, strlen(buf), buf, sizeof(buf));

  printf("lines: %d\n", atoi(buf));

  // run du to get the size
  res = run_du(files_buf, nfiles, buf, sizeof(buf));

  printf("got (%.*s)\n", res, buf);
  char* lines[100] = {};
  int nlines = get_lines(buf, lines);
  int total = 0;
  for (int i = 0; i < nlines - 1; ++i) {
    int size = atoi(lines[i]);
    total += size;
  }

  printf("total size: %d", total);
}
