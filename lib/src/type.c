#include "type.h"

#include <gc.h>
#include <stddef.h>
#include "types/bool.h"

plsm_type_t *plsm_maketype(plsm_type_t *base, char *name, uint32_t size)
{
  plsm_type_t *type = GC_malloc(sizeof(plsm_type_t));
  if (type == NULL)
    DIE_ERRNO("GC_malloc");

  type->base = base;
  type->name = name;
  type->size = size;

  return type;
}

TYPE(Bool)
plsm_instanceof(plsm_base_t *value, plsm_type_t *type)
{
  for (plsm_type_t *t = value->type; t != NULL; t = t->base)
    if (t == type)
      return MAKE(Bool)(1);

  return MAKE(Bool)(0);
}
