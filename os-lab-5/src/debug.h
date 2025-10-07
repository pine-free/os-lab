#pragma once

#include <stdio.h>
#include <stdlib.h>

extern int DEBUG;

int set_debug();

#define INFO_PRINT(label, ...)                                                 \
  {                                                                            \
    printf("[INFO - %s] ", label);                                             \
    printf(__VA_ARGS__);                                                       \
    printf("\n");                                                              \
  }

#define DBG_PRINT(...)                                                         \
  {                                                                            \
    printf("[DEBUG - %s:%d - %s] ", __BASE_FILE__, __LINE__, __FUNCTION__);    \
    printf(__VA_ARGS__);                                                       \
    printf("\n");                                                              \
  }
