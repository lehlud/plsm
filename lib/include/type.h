#pragma once

#include "macros.h"

typedef struct type_t type_t;

typedef struct type_t
{
  type_t *base;
  int opaque;
  char *name;
} type_t;

type_t *LIB(makeType)(type_t *base, int opaque, char *name);
