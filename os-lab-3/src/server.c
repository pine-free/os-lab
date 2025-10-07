#include "common.h"
#include "messages.h"

int main() {
  if (getenv("DEBUG")) {
    DEBUG = 1;
  }

  int qid = open_queue(MSG_QUEUE_KEY);

  struct mymsgbuf msg;
  while (1) {
    read_message(qid, NONE, &msg);
  }
}
