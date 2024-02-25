#include "types/int.h"

#include <gc.h>
#include "type.h"
#include "types/bool.h"

DEFMAKETYPE(Int)
()
{
  static plsm_type_t *type = NULL;
  if (type)
    return type;

  return (type = plsm_maketype(NULL, "Int", TYPESIZE(Int)));
}

DEFMAKE(Int)
(int64_t value)
{
  return (TYPE(Int)){
      .type = NULL,
      .value = value,
  };
}

DEFUNOP(OP_ADD, Int, Int)
{
  return *it;
}

DEFUNOP(OP_SUB, Int, Int)
{
  return MAKE(Int)(it->value * -1);
}

DEFBINOP(OP_ADD, Int, Int, Int)
{
  return MAKE(Int)(it->value + other->value);
}

DEFBINOP(OP_SUB, Int, Int, Int)
{
  return MAKE(Int)(it->value - other->value);
}

DEFBINOP(OP_MUL, Int, Int, Int)
{
  return MAKE(Int)(it->value * other->value);
}

DEFBINOP(OP_DIV, Int, Int, Int)
{
  return MAKE(Int)(it->value / other->value);
}

DEFBINOP(OP_MOD, Int, Int, Int)
{
  return MAKE(Int)(it->value % other->value);
}

DEFBINOP(OP_EQ, Bool, Int, Int)
{
  return MAKE(Bool)((intptr_t)(it->value == other->value));
}
