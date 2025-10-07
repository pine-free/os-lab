#include "sockets.h"
#include "common.h"
#include "debug.h"
#include <sys/un.h>

const int S_DOMAIN = AF_UNIX;
const int S_TYPE = SOCK_DGRAM;
const int S_PROTOCOL = 0;
const int N_CONNECTIONS = 5;
const char SRV_PATH[] = "unix_socket";


int get_sockfd() {
  int sock;
  OR_DIE((sock = socket(S_DOMAIN, S_DOMAIN, S_PROTOCOL)));

  return sock;
}

struct sockaddr_un get_sockaddr() {
  struct sockaddr_un addr;
  addr.sun_family = S_DOMAIN;
  strcpy(addr.sun_path, "unix_socket");
  return addr;
}

struct unix_socket get_socket() {
  if (DEBUG) {
    DBG_PRINT("creating unix socket");
  }
  struct unix_socket sock;
  sock.sfd = get_sockfd();
  sock.addr = get_sockaddr();
  sock.addr_size = sizeof(sock.addr);
  if (DEBUG) {
    DBG_PRINT("created unix socket");
    DBG_PRINT("fd %d", sock.sfd);
    DBG_PRINT("addr %s", sock.addr.sun_path);
  }
  return sock;
}

int unix_bind(struct unix_socket *sock) {
  if (DEBUG) {
    DBG_PRINT("binding socket to addr %s", sock->addr.sun_path);
  }
  bind(sock->sfd, (struct sockaddr *)&sock->addr, sizeof(sock->addr));
}

int unix_listen(struct unix_socket *sock) {
  if (DEBUG) {
    DBG_PRINT("listening on %s, max connections: %d", sock->addr.sun_path,
              N_CONNECTIONS);
  }
  listen(sock->sfd, N_CONNECTIONS);
}

int unix_connect(struct unix_socket *sock) {
  if (DEBUG) {
    DBG_PRINT("connecting to %s", sock->addr.sun_path);
  }
  return connect(sock->sfd, (struct sockaddr *)&sock->addr, sizeof(sock->addr));
}

int unix_accept(struct unix_socket *srv_sock, struct unix_socket *client_sock) {
  client_sock->sfd =
      accept(srv_sock->sfd, (struct sockaddr *)&client_sock->addr,
             &client_sock->addr_size);
  if (DEBUG) {
    if (client_sock->sfd != -1) {
      DBG_PRINT("accepted connection from %s", client_sock->addr.sun_path);
    }
  }
  return client_sock->sfd;
}
