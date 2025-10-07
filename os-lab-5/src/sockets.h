#pragma once

#include "sys/socket.h"
#include "sys/un.h"

const int S_DOMAIN = AF_UNIX;
const int S_TYPE = SOCK_DGRAM;
const int S_PROTOCOL = 6;
const int N_CONNECTIONS = 5;
const char SRV_PATH[] = "unix_socket";

struct unix_socket {
  struct sockaddr_un addr;
  socklen_t addr_size;
  int sfd;
};

struct unix_socket get_socket();
int unix_bind(struct unix_socket *sock);
int unix_listen(struct unix_socket *sock);
int unix_connect(struct unix_socket *sock);
int unix_accept(struct unix_socket *srv_sock, struct unix_socket *client_sock);
