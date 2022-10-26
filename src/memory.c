#include <stdio.h>
#include <stdlib.h>

#include "memory.h"
#include "inst.h"

memory_t init_memory(inst_t *insts)
{
  memory_t memory;

  memory.stack_size = 0;
  memory.stack = malloc(0);
  memory.sp = memory.stack;

  memory.insts = insts;
  memory.ip = memory.insts;

  return memory;
}

stack_el_t stack_pop(memory_t *memory)
{
  stack_el_t res = *memory->sp;

  memory->stack = realloc(memory->stack, (--memory->stack_size) * sizeof(stack_el_t));
  memory->sp = &memory->stack[memory->stack_size - 1];

  return res;
}

void stack_push(memory_t *memory, stack_el_t value)
{
  memory->stack = realloc(memory->stack, (++memory->stack_size) * sizeof(stack_el_t));
  memory->sp = &(memory->stack[memory->stack_size - 1]);

  *memory->sp = value;
}
