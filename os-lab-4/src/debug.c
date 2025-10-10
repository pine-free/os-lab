#include "debug.h"

int set_debug() {
  if (getenv("DEBUG")) {
    DEBUG = 1;
  }

  return DEBUG;
}
