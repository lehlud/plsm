#pragma once

#include "macros.h"

typedef struct plsm_type_t plsm_type_t;

typedef struct plsm_type_t
{
  plsm_type_t *base;
  char *name;
  uint32_t size;
} plsm_type_t;

plsm_type_t *plsm_maketype(plsm_type_t *base, char *name, uint32_t size);

typedef struct plsm_base_t
{
  plsm_type_t *type;
} plsm_base_t;

typedef struct TYPE(Bool) TYPE(Bool);

TYPE(Bool)
plsm_instanceof(plsm_base_t *value, plsm_type_t *type);
