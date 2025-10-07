#include "common.h"
#include "sockets.h"

#define CLIENT_PRINT(...) INFO_PRINT("client", __VA_ARGS__)

struct myclient {
  struct unix_socket sock;
};

struct myclient get_client() {
  if (DEBUG) {
    DBG_PRINT("create client")
  }
  struct myclient client = {get_socket()};
  return client;
}


int main() {
  set_debug();
  CLIENT_PRINT("starting client");

  struct myclient client = get_client();
  unix_connect(&client.sock);

  return 0;
}
