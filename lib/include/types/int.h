#pragma once

#include "macros.h"
#include "type.h"

#include <gc.h>

typedef struct TYPE(Int)
{
  plsm_type_t *type;
  int64_t value;
} TYPE(Int);

DEFMAKE(Int)
(int64_t value);

DEFBINOP(BINOP_ADD, Int, Int, Int);
DEFBINOP(BINOP_SUB, Int, Int, Int);
DEFBINOP(BINOP_MUL, Int, Int, Int);
DEFBINOP(BINOP_DIV, Int, Int, Int);
DEFBINOP(BINOP_MOD, Int, Int, Int);

DEFBINOP(BINOP_EQ, Bool, Int, Int);
