#include "common.h"
#include "sockets.h"

#define SERVER_PRINT(...) INFO_PRINT("server", __VA_ARGS__)

struct myserver {
  struct unix_socket sock;
};


int get_server(struct myserver *server) {
  if (DEBUG) {
    DBG_PRINT("create server");
  }

  get_socket(&server->sock);
  return 1;
}

int main() {
  set_debug();
  SERVER_PRINT("starting server");
  struct myserver server;
  get_server(&server);
  OR_DIE(unix_bind(&server.sock));
  OR_DIE(unix_listen(&server.sock));

  struct unix_socket client_sock;

  while (1) {
    if (unix_accept(&server.sock, &client_sock) != -1) {
      SERVER_PRINT("accepted connection!");
    }
    
  }
}
