#include "common.h"

#define SERVER_PRINT(...) INFO_PRINT("server", __VA_ARGS__)


int main() {
  set_debug();
  SERVER_PRINT("starting server");

  return 0;
}
