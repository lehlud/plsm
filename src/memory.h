#pragma once

#include <stdint.h>

#include "inst.h"

typedef uint64_t stack_el_t;

typedef struct memory_t
{
  stack_el_t *sp;
  stack_el_t *stack;
  uint64_t stack_size;

  inst_t *ip;
  inst_t *insts;
} memory_t;

memory_t init_memory(inst_t *insts);

stack_el_t stack_pop(memory_t *memory);
void stack_push(memory_t *memory, stack_el_t value);
