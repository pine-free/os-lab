#include "common.h"
#include "debug.h"
#include "messages.h"
#include <stdio.h>
#include <sys/msg.h>

int run_fgrep(char **files, int nfiles, char *wbuf, int wsize) {
  const char *fgrep_path = "/usr/bin/grep";
  char *fgrep_args[] = {"grep", "-Fl", "awk"};
  append_arr(2, fgrep_args, files, nfiles);
  fgrep_args[2 + nfiles] = NULL;

  int res = run_subprocess_nopipe(fgrep_path, fgrep_args, wbuf, wsize);
  wbuf[--res] = 0;

  if (DEBUG) {
    DBG_PRINT("fgrep got '%.*s'", res, wbuf);
  }

  return res;
}

int get_awk_files(char *msg_data, char **awk_files) {
  char *files_buf[100];
  int nfiles = get_lines(msg_data, files_buf);

  char buf[1024];

  int fgrep_out_len = run_fgrep(files_buf, nfiles, buf, sizeof(buf));
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
      puts("no files from msg contain invocation of awk\n");
    } else {
      printf("got %d files with awk invocations\n", nawk_files);
      for (int i = 0; i < nawk_files; ++i) {
        printf("%d. %s\n", i + 1, awk_files[i]);
      }
    }

    struct msqid_ds msg_info;
    msgctl(qid, IPC_STAT, &msg_info);
    DBG_PRINT("queue contains %lu more messages", msg_info.msg_qnum);

    do {
      msg_size = read_message(qid, NONE, &msg, 0);
      total_msgs_size += msg_size;
    } while(msg.mtype != BYTES_SIZE);

    printf("total size of read messages: %d bytes\n", total_msgs_size);
    total_msgs_size = 0;
  }
}
