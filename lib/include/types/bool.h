#pragma once

#include "macros.h"
#include "type.h"

#include <stdint.h>

typedef struct TYPE(Bool)
{
  plsm_type_t *type;
  uint8_t value;
} TYPE(Bool);

DEFMAKE(Bool)
(intptr_t value);
