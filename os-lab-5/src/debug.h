#pragma once

#include <stdio.h>
#include <stdlib.h>

extern int DEBUG;

int set_debug();

#define LOG_PRINT_DETAILED(type, buf, ...)                                     \
  {                                                                            \
    fprintf(buf, "[%s - %s:%d - %s] ", type, __BASE_FILE__, __LINE__,          \
            __FUNCTION__);                                                     \
    fprintf(buf, __VA_ARGS__);                                                 \
    fprintf(buf, "\n");                                                        \
  }

#define INFO_PRINT(label, ...)                                                 \
  {                                                                            \
    printf("[INFO - %s] ", label);                                             \
    printf(__VA_ARGS__);                                                       \
    printf("\n");                                                              \
  }

#define DBG_PRINT(...) LOG_PRINT_DETAILED("DEBUG", stdout, __VA_ARGS__)
#define ERR_PRINT(...) LOG_PRINT_DETAILED("ERROR", stderr, __VA_ARGS__)
