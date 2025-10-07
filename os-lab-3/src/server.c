#include "common.h"
#include "debug.h"
#include "messages.h"
#include <stdio.h>
#include <sys/msg.h>

int run_grep(char **files, int nfiles, char *wbuf, int wsize) {
  const char *grep_path = "/usr/bin/grep";
  char *grep_args[100] = {"grep", "-Fl", "awk"};
  int arr_len = 3;
  append_arr(arr_len, grep_args, files, nfiles);
  grep_args[arr_len + nfiles] = NULL;

  for (int i = 0; i < arr_len + nfiles; ++i) {
    DBG_PRINT("grep arg: %s", grep_args[i]);
  }
  int res = run_subprocess_nopipe(grep_path, grep_args, wbuf, wsize);
  wbuf[--res] = 0;

  if (DEBUG) {
    DBG_PRINT("grep got '%.*s'", res, wbuf);
  }

  return res;
}

int get_awk_files(char *msg_data, char **awk_files) {
  char *files_buf[100];
  int nfiles = get_lines(msg_data, files_buf);
  DBG_PRINT("got %d files", nfiles);

  char buf[1024];

  int fgrep_out_len = run_grep(files_buf, nfiles, buf, sizeof(buf));
  if (fgrep_out_len == -1) {
    DBG_PRINT("grep got no files, exiting");
    return 0;
  }

  int nawk_files = get_lines(buf, awk_files);

  return nawk_files;
}

int main() {
  if (getenv("DEBUG")) {
    DEBUG = 1;
  }

  int qid = open_queue(MSG_QUEUE_KEY);

  struct mymsgbuf msg;
  char *awk_files[100] = {};
  int msg_size = 0;
  int total_msgs_size = 0;

  while (1) {
    if ((msg_size = read_message(qid, NONE, &msg, 0)) <= 0)
      continue;

    if (msg.mtype != FILES) continue;

    DBG_PRINT("=== GOT MESSAGE, START PROCESING ===");
    DBG_PRINT("read message type: %d", msg.mtype);
    total_msgs_size += msg_size;

    int nawk_files = get_awk_files(msg.msg_data, awk_files);
    if (nawk_files == 0) {
      puts("no files from msg contain invocation of awk");
    } else {
      printf("got %d files with awk invocations\n", nawk_files);
      for (int i = 0; i < nawk_files; ++i) {
        printf("%d. %s\n", i + 1, awk_files[i]);
      }
    }

    struct msqid_ds msg_info;
    msgctl(qid, IPC_STAT, &msg_info);
    DBG_PRINT("reading until bytes size message");
    do {
      msg_size = read_message(qid, NONE, &msg, 0);
      total_msgs_size += msg_size;
    } while(msg.mtype != BYTES_SIZE);

    printf("total size of read messages: %d bytes\n", total_msgs_size);
    total_msgs_size = 0;
  }
}
