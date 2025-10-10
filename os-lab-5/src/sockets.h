#pragma once

#include "sys/socket.h"
#include "sys/un.h"

struct unix_socket {
  struct sockaddr_un addr;
  socklen_t addr_size;
  int sfd;
};

int init_socket(struct unix_socket *sock);
int unix_bind(struct unix_socket *sock);
int unix_listen(struct unix_socket *sock);
int unix_connect(struct unix_socket *sock);
int unix_accept(struct unix_socket *srv_sock, struct unix_socket *client_sock);
int unix_read(struct unix_socket *sock, void *buf, size_t n);
int unix_write(struct unix_socket *sock, const void *buf, size_t n);
