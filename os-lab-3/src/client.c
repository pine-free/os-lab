#include "common.h"

int run_find(char *buf, int size) {
  const char *find_path = "/usr/bin/find";
  char *const find_args[] = {"find", ".", "-name", "*.sh", NULL};

  int res = run_subprocess_nopipe(find_path, find_args, buf, size);
  buf[--res] = 0;
  
  if (DEBUG) {
    DBG_PRINT("find output '%.*s'\n", res, buf);
  }
  return res;
}

int run_wc(char **files, int nfiles, char *wbuf, int wsize) {
  const char *wc_path = "/usr/bin/wc";
  char *wc_args[100] = {"wc", "-l"};
  append_arr(2, wc_args, files, nfiles);
  wc_args[2 + nfiles] = NULL;

  int res = run_subprocess_nopipe(wc_path, wc_args, wbuf, wsize);
  wbuf[--res] = 0;

  if (DEBUG) {
    DBG_PRINT("wc got '%.*s'\n", res, wbuf);
  }
  return res;
}

int run_tail(const char *rbuf, int rsize, char *wbuf, int wsize) {
  const char *tail_path = "/usr/bin/tail";
  char *tail_args[] = {"tail", "-n", "1", NULL};

  if (DEBUG) {
    DBG_PRINT("tail stdin: '%.*s'\n", rsize, rbuf);
  }
  int res = run_subprocess(tail_path, tail_args, rbuf, rsize, wbuf, wsize);
  wbuf[--res] = 0;
  if (DEBUG) {
    DBG_PRINT("tail got '%.*s'\n", res, wbuf);
  }
  return res;
}

int run_du(char **files, int nfiles, char *wbuf, int wsize) {
  if (nfiles == 0) {
    if (DEBUG) {
      DBG_PRINT("du got no files on input, size 0");
    }
    return 0;
  }

  const char* du_path = "/usr/bin/du";
  char* du_args[100] = {"du",  "-b"};
  append_arr(2, du_args, files, nfiles);
  du_args[2 + nfiles] = NULL;

  int res = run_subprocess_nopipe(du_path, du_args, wbuf, wsize);
  wbuf[--res] = 0;
  if (DEBUG) {
    DBG_PRINT("du got '%.*s'\n", res, wbuf);
  }
  return res;
}

int fill_files_buf(char* files_buf[]) {
  char buf[1024] = {};
  int res = run_find(buf, sizeof(buf));

  int nfiles = get_lines(buf, files_buf);

  if (DEBUG) {
    DBG_PRINT("got %d files\n", nfiles);
    for (int i = 0; i < nfiles; ++i) {
      DBG_PRINT("%d. %s\n", i + 1, files_buf[i]);
    }
  }

  return nfiles;
}

int get_lines_count(char* files_buf[], int nfiles) {
  char buf[1024];
  int res = run_wc(files_buf, nfiles, buf, sizeof(buf));
  res = run_tail(buf, strlen(buf), buf, sizeof(buf));

  return atoi(buf);
}

int get_bytes_size(char* files_buf[], int nfiles) {
  char buf[1024];

  int res = run_du(files_buf, nfiles, buf, sizeof(buf));

  char* lines[100] = {};
  int nlines = get_lines(buf, lines);
  int total = 0;
  for (int i = 0; i < nlines; ++i) {
    int size = atoi(lines[i]);
    total += size;
  }

  return total;
}

int main() {
  if (getenv("DEBUG")) {
    DEBUG=1;
  }

  char* files_buf[100];
  int nfiles = fill_files_buf(files_buf);

  for (int i = 0; i < nfiles; ++i) {
    printf("%s\n", files_buf[i]);
  }

  int lines_count = get_lines_count(files_buf, nfiles);
  int bytes_size = get_bytes_size(files_buf, nfiles);


  printf("lines: %d\n", lines_count);
  printf("total size: %d", bytes_size);
}
