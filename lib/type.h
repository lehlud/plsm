#include "lib.h"
#include <stdlib.h>

typedef struct _PLSMID(value) _PLSMID(value);

typedef _PLSMID(value) * (*_PLSMID(repr))(_PLSMID(value) * value);
typedef _PLSMID(value) *
    (*_PLSMID(operator))(_PLSMID(value) * a, _PLSMID(value) * b);

typedef struct _PLSMID(type) {
  _PLSMID(string) name;
  _PLSMID(operator) * operators;
  _PLSMID(repr) repr;
} _PLSMID(type);

#define _PLSM_OPADD 0
#define _PLSM_OPSUB 1
#define _PLSM_OPMUL 2
#define _PLSM_OPDIV 3

inline _PLSMID(type) * _PLSMID(maketype)(_PLSMID(string) name) {
  _PLSMID(type) *type = (_PLSMID(type) *)malloc(sizeof(_PLSMID(type)));

  type->name = name;
  type->operators = (_PLSMID(operator) *)calloc(200, sizeof(_PLSMID(operator)));

  return type;
}

#define _PLSMMAKETYPE(id) __plsm_maketype_##id
#define _PLSMREPRID(id) __plsm_repr_##id
#define _PLSMTYPEOPID(id, op) __plsm_typeop_##id##_##op
#define _PLSMTYPEOPSIGN(id, op)                                                \
  _PLSMID(value) * _PLSMTYPEOPID(id, op)(_PLSMID(value) * a, _PLSMID(value) * b)

#define _PLSMSTATICTYPENAME(id) type_##id
#define _PLSMSTATICTYPE(id) static _PLSMID(type) * _PLSMSTATICTYPENAME(id);
