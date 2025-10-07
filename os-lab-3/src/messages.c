#include "messages.h"
#include "debug.h"

int open_queue(key_t msg_key) {
  int qid;
  if ((qid = msgget(msg_key, IPC_CREAT | 0660)) == -1) {
    return -1;
  }
  if (DEBUG) {
    DBG_PRINT("opened queue with id %d", MSG_QUEUE_KEY);
  }
  return qid;
}

int send_message(int qid, struct mymsgbuf *qbuf) {
  int res, len;

  // The length of the message if the size of the entire struct
  // minus the type
  len = sizeof(struct mymsgbuf) - sizeof(long);
  if ((res = msgsnd(qid, qbuf, len, 0)) == -1) {
    return -1;
  }
  if (DEBUG) {
    DBG_PRINT("sent message: '%s'", qbuf->msg_data);
  }
  return res;
}

int read_message(int qid, long type, struct mymsgbuf *qbuf) {
  int res, len;

  // The length of the message if the size of the entire struct
  // minus the type
  len = sizeof(struct mymsgbuf) - sizeof(long);
  if ((res = msgrcv(qid, qbuf, len, type, 0)) == -1) {
    return -1;
  }
  if (DEBUG) {
    DBG_PRINT("got message: '%s'", qbuf->msg_data);
  }
  return res;
}
