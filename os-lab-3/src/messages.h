#pragma once

#include "debug.h"
#include <sys/msg.h>

extern key_t MSG_QUEUE_KEY;

enum MSG_TYPE {
  NONE = 0,
  FILES,
  LINE_COUNT,
  BYTES_SIZE
};

struct mymsgbuf {
  enum MSG_TYPE mtype;
  char msg_data[1024];
};

int open_queue(key_t msg_key);

int send_message(int qid, struct mymsgbuf *qbuf);

int read_message(int qid, enum MSG_TYPE type, struct mymsgbuf *qbuf, int msgflag);
