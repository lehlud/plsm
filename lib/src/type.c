#include "type.h"

#include <stdlib.h>

type_t *LIB(makeType)(type_t *base, int opaque, char *name)
{
  type_t *type = malloc(sizeof(type_t));
  if (!type)
    DIE_ERRNO("malloc");

  type->base = base;
  type->opaque = opaque;
  type->name = name;

  return type;
}
