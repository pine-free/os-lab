#include "common.h"

#define CLIENT_PRINT(...) INFO_PRINT("client", __VA_ARGS__)

int main() {
  set_debug();
  CLIENT_PRINT("starting client");

  return 0;
}
