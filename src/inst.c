#include <stdio.h>
#include <stdlib.h>

#include "inst.h"
#include "memory.h"

inst_handler_t *inst_handlers;
void init_insts()
{
  inst_handlers = malloc(255 * sizeof(inst_handler_t));

  inst_handlers[INST_NOP] = inst_nop;
  inst_handlers[INST_HLT] = inst_hlt;

  inst_handlers[INST_IADD] = inst_iadd;
  inst_handlers[INST_FADD] = inst_fadd;

  inst_handlers[INST_NEG] = inst_neg;

  inst_handlers[INST_LOAD] = inst_load;

  inst_handlers[INST_JMP] = inst_jmp;
  inst_handlers[INST_JMPIF] = inst_jmpif;
}

void next_inst(memory_t *memory)
{
  inst_handlers[memory->ip->op](memory);
  memory->ip++;
}

void inst_nop(memory_t *memory) {}
void inst_hlt(memory_t *memory)
{
  exit(stack_pop(memory));
}

void inst_iadd(memory_t *memory)
{
  uint64_t a = stack_pop(memory), b = stack_pop(memory);
  stack_push(memory, a + b);
}

void inst_fadd(memory_t *memory)
{
  uint64_t a = stack_pop(memory), b = stack_pop(memory);
  _Float64 c = *(_Float64 *)&a, d = *(_Float64 *)&b;
  _Float64 f = c + d;

  stack_push(memory, *(uint64_t *)&f);
}

void inst_neg(memory_t *memory)
{
  uint64_t a = stack_pop(memory);
  stack_push(memory, ~a);
}

void inst_load(memory_t *memory)
{
  stack_push(memory, memory->ip->data);
}

void inst_jmp(memory_t *memory) {}
void inst_jmpif(memory_t *memory) {}
