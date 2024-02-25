#include "types/bool.h"

#include <gc.h>

DEFMAKE(Bool)
(intptr_t value)
{
  return (TYPE(Bool)){
      .type = NULL,
      .value = value != 0,
  };
}

DEFUNOP(OP_NOT, Bool, Bool)
{
  return MAKE(Bool)(!it->value);
}

DEFBINOP(OP_LAND, Bool, Bool, Bool)
{
  return MAKE(Bool)(it->value && other->value);
}

DEFBINOP(OP_LOR, Bool, Bool, Bool)
{
  return MAKE(Bool)(it->value || other->value);
}
