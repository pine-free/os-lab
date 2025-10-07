#include "sockets.h"
#include "debug.h"
#include <sys/un.h>

int get_sockfd() {
  int sock = socket(S_DOMAIN, S_DOMAIN, S_PROTOCOL);
  return sock;  
}

struct sockaddr_un get_sockaddr() {
  struct sockaddr_un addr;
  strcpy(addr.sun_path, SRV_PATH);
  addr.sun_family = S_DOMAIN;
  return addr;
}

struct unix_socket get_socket() {
  if (DEBUG) {
    DBG_PRINT("creating unix socket");
  }
  struct unix_socket sock;
  sock.sfd = get_sockfd();
  sock.addr = get_sockaddr();
  return sock;
}

int unix_bind(struct unix_socket *sock) {
  if (DEBUG) {
    DBG_PRINT("binding socket to addr %s", sock->addr.sun_path);
  }
  bind(sock->sfd, (struct sockaddr*)&sock->addr, sizeof(sock->addr));
}


int unix_listen(struct unix_socket *sock) {
  if (DEBUG) {
    DBG_PRINT("listening on %s, max connections: %d", sock->addr.sun_path, N_CONNECTIONS);
  }
  listen(sock->sfd, N_CONNECTIONS);
}

int unix_connect(struct unix_socket *sock) {
  if (DEBUG) {
    DBG_PRINT("connecting to %s", sock->addr.sun_path);
  }
  return connect(sock->sfd, (struct sockaddr*) &sock->addr, sizeof(sock->addr));
}

