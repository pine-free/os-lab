#include "common.h"
#include "sockets.h"

#define CLIENT_PRINT(...) INFO_PRINT("client", __VA_ARGS__)

struct myclient {
  struct unix_socket srv_sock;
};

int get_client(struct myclient* client) {
  if (DEBUG) {
    DBG_PRINT("create client")
  }
  get_socket(&client->srv_sock);
  return 1;
}


int main() {
  set_debug();
  CLIENT_PRINT("starting client");

  struct myclient client;
  get_client(&client);
  OR_DIE(unix_connect(&client.srv_sock));

  return 0;
}
