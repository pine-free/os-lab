#pragma once

#include <stdio.h>

extern int DEBUG;

#define DBG_PRINT(...) { \
  printf("[DEBUG - %s:%d - %s] ", __BASE_FILE__, __LINE__, __FUNCTION__); \
  printf(__VA_ARGS__); \
  printf("\n"); \
}
