#include "messages.h"
#include "debug.h"

key_t MSG_QUEUE_KEY = 1337;

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

const int MSG_LEN = sizeof(struct mymsgbuf) - sizeof(enum MSG_TYPE);

int send_message(int qid, struct mymsgbuf *qbuf) {
  int res, len;

  // The length of the message if the size of the entire struct
  // minus the type
  if ((res = msgsnd(qid, qbuf, MSG_LEN, 0)) == -1) {
    return -1;
  }
  if (DEBUG) {
    DBG_PRINT("sent message: '%s'", qbuf->msg_data);
  }
  return res;
}

int read_message(int qid, enum MSG_TYPE type, struct mymsgbuf *qbuf, int msgflg) {
  int res, len;

  // The length of the message if the size of the entire struct
  // minus the type
  if ((res = msgrcv(qid, qbuf, MSG_LEN, type, msgflg)) == -1) {
    return -1;
  }
  if (DEBUG) {
    DBG_PRINT("got message: '%s'", qbuf->msg_data);
  }
  return res;
}
