#pragma once

#include "debug.h"
#include <sys/msg.h>

extern key_t MSG_QUEUE_KEY;

struct mymsgbuf {
  long mtype;
  char msg_data[1024];
};

int open_queue(key_t msg_key);

int send_message(int qid, struct mymsgbuf *qbuf);

int read_message(int qid, long type, struct mymsgbuf *qbuf);
