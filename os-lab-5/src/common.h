# pragma once

#include "debug.h"
#include "messages.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int get_lines(char *, char *[]);
int run_subprocess(const char *path, char *const args[], const char *rbuf,
                   int rsize, char *wbuf, int wsize);
int run_subprocess_nopipe(const char *path, char *const args[], char *wbuf,
                          int wsize);
void append_arr(int start, char* dst[], char* src[], int src_size);
