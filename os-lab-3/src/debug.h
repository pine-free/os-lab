#pragma once

#include <stdio.h>

extern int DEBUG;

#define DBG_PRINT(...) { \
  printf("[DEBUG - %s] ", __FUNCTION__); \
  printf(__VA_ARGS__); \
  printf("\n"); \
}
