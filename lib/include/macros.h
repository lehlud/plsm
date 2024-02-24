#pragma once

#include <stdlib.h>
#include <stdio.h>

#define DIE             \
  {                     \
    exit(EXIT_FAILURE); \
  }

#define DIE_ERRNO(action) \
  {                       \
    perror(action);       \
    DIE;                  \
  }

#define MODINIT(module) void plsmmod_##module##_init()

#define LIB(name) plsmlib_##name
