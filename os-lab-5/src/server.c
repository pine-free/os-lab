#include "common.h"
#include "sockets.h"
#include "stdlib.h"

#define SERVER_PRINT(...) INFO_PRINT("server", __VA_ARGS__)

struct myserver {
  struct unix_socket sock;
};

int init_server(struct myserver *server) {
  if (DEBUG) {
    DBG_PRINT("create server");
  }

  init_socket(&server->sock);
  return 1;
}

int get_proc_info(char *rbuf, int rsize) {
  char *lines[100];
  int nprocs = get_lines(rbuf, lines);

  char *extra_args[100];

  for (int i = 0; i < nprocs; ++i) {
    extra_args[i] = malloc(10);
    sprintf(extra_args[i], "p%d", atoi(lines[i]));
  }

  char *ps_args[100] = {"ps", "-o", "pid,time,pri"};
  append_arr(3, ps_args, extra_args, nprocs);
  if (DEBUG) {
    for (int i = 0; i < 3 + nprocs; ++i) {
      DBG_PRINT("arg: %s", ps_args[i]);
    }
  }

  ps_args[nprocs + 3] = NULL;

  char output[1024];
  run("/usr/bin/ps", ps_args, output, sizeof(output));
  SERVER_PRINT("%s", output);

  for (int i = 0; i < nprocs; ++i) {
    free(extra_args[i]);
  }

  return 0;
}

int main() {
  set_debug();
  SERVER_PRINT("starting server");
  struct myserver server;
  init_server(&server);
  OR_DIE(unix_bind(&server.sock));
  OR_DIE(unix_listen(&server.sock));

  struct unix_socket client_sock;
  char msg[1024];

  while (1) {
    if (unix_accept(&server.sock, &client_sock) == -1) {
      continue;
    }

    SERVER_PRINT("accepted connection!");
    unix_read(&client_sock, msg, sizeof(msg));
    get_proc_info(msg, sizeof(msg));
  }
}
