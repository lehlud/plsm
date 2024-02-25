#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define DIE             \
  {                     \
    exit(EXIT_FAILURE); \
  }

#define DIE_ERRNO(action) \
  {                       \
    perror(action);       \
    DIE;                  \
  }

#define TYPE(type) plsm_##type##_t
#define TYPESIZE(type) (sizeof(TYPE(type)) - sizeof(plsm_base_t))

#define MAKETYPE(type) plsm_maketype_##type
#define DEFMAKETYPE(type) plsm_type_t *MAKETYPE(type)

#define MAKE(type) plsm_make_##type
#define DEFMAKE(type) TYPE(type) MAKE(type)

#define OP_ADD add
#define OP_SUB sub
#define OP_MUL mul
#define OP_DIV div
#define OP_MOD mod
#define OP_EQ eq
#define OP_NEQ neq
#define OP_LAND land
#define OP_LOR lor
#define OP_GT gt
#define OP_GTE gte
#define OP_LT lt
#define OP_LTE lte
#define OP_NOT not

#define UNOP(action, intype) plsm_unop_##action##_##intype
#define DEFUNOP(action, restype, intype) TYPE(restype) UNOP(action, intype)(TYPE(intype) * it)

#define BINOP(action, ltype, rtype) plsm_binop_##action##_##ltype##_##rtype
#define DEFBINOP(action, restype, ltype, rtype) \
  TYPE(restype)                                 \
  BINOP(action, ltype, rtype)                   \
  (TYPE(ltype) * it, TYPE(rtype) * other)
