#pragma once

#include "debug.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define OR_DIE(expr)                                                           \
  {                                                                            \
    if ((expr) == -1) {                                                        \
      die(__FUNCTION__);                                                       \
    }                                                                          \
  }

#define OR_FAIL(expr)                                                          \
  {                                                                            \
    if ((expr) == -1) {                                                        \
      print_err(__FUNCTION__);                                                 \
      return -1;                                                               \
    }                                                                          \
  }

#define RUN(path, buf, ...)                                                    \
  {                                                                            \
    char *_args[] = {__VA_ARGS__, NULL};                                       \
    run(path, _args, buf, sizeof(buf));                                        \
  }

#define RPIPE(path, rbuf, wbuf, ...)                                           \
  {                                                                            \
    char *_args[] = {__VA_ARGS__, NULL};                                       \
    rpipe(path, _args, rbuf, sizeof(rbuf), wbuf, sizeof(wbuf));                \
  }

int get_lines(char *, char *[]);
int rpipe(const char *path, char *const args[], char *rbuf, int rsize,
          char *wbuf, int wsize);
int run(const char *path, char *const args[], char *wbuf, int wsize);
void append_arr(int start, char *dst[], char *src[], int src_size);
void die(const char *msg);
void print_err(const char *msg);
