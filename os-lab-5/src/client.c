#include "common.h"
#include "sockets.h"
#include <sys/un.h>

#define CLIENT_PRINT(...) INFO_PRINT("client", __VA_ARGS__)

struct myclient {
  struct unix_socket srv_sock;
};

int init_client(struct myclient* client) {
  if (DEBUG) {
    DBG_PRINT("create client")
  }
  init_socket(&client->srv_sock);
  return 1;
}

int get_sleeping_procs(char* wbuf, int wsize) {
  char buf[1024];
  RUN("/usr/bin/ps", buf, "ps", "-eo", "pid=,s=");
  RPIPE("/usr/bin/grep", buf, buf, "grep", "S");
  strncpy(wbuf, buf, wsize);
}

int main() {
  set_debug();
  CLIENT_PRINT("starting client");

  struct myclient client;
  init_client(&client);
  OR_DIE(unix_connect(&client.srv_sock));

  char sleeping_procs[1024];

  get_sleeping_procs(sleeping_procs, sizeof(sleeping_procs));

  return 0;
}
