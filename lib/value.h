#include "lib.h"

typedef struct _PLSMID(type) _PLSMID(type);

typedef struct _PLSMID(value) {
  void *data;
  _PLSMID(type) * type;
} _PLSMID(value);

#define _PLSMMAKE(id) __plsm_make_##id
