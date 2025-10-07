#include "common.h"
#include "sockets.h"

#define SERVER_PRINT(...) INFO_PRINT("server", __VA_ARGS__)

struct myserver {
  struct unix_socket sock;
};

struct myserver get_server() {
  if (DEBUG) {
    DBG_PRINT("create server");
  }
  struct myserver server = {get_socket()};
}

int main() {
  set_debug();
  SERVER_PRINT("starting server");
  struct myserver server = get_server();
  unix_bind(&server.sock);
  unix_listen(&server.sock);

  struct unix_socket client_sock = get_socket();

  while (1) {
    unix_accept(&server.sock, &client_sock);
  }
}
