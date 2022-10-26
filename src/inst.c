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

  inst_handlers[INST_ICASTF] = inst_icastf;
  inst_handlers[INST_FCASTI] = inst_fcasti;

  inst_handlers[INST_CMPEQ] = inst_cmpeq;
  inst_handlers[INST_ICMPLT] = inst_icmplt;
  inst_handlers[INST_ICMPGT] = inst_icmpgt;
  inst_handlers[INST_FCMPLT] = inst_fcmplt;
  inst_handlers[INST_FCMPGT] = inst_fcmpgt;

  inst_handlers[INST_NEG] = inst_neg;
  inst_handlers[INST_AND] = inst_and;
  inst_handlers[INST_OR] = inst_or;
  inst_handlers[INST_XOR] = inst_xor;

  inst_handlers[INST_LOAD] = inst_load;

  inst_handlers[INST_JMP] = inst_jmp;
  inst_handlers[INST_JMPTRUE] = inst_jmptrue;
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
  stack_el_t a = stack_pop(memory), b = stack_pop(memory);
  stack_push(memory, a + b);
}

void inst_fadd(memory_t *memory)
{
  stack_el_t a = stack_pop(memory), b = stack_pop(memory);
  _Float64 c = *(_Float64 *)&a, d = *(_Float64 *)&b;
  _Float64 f = c + d;

  stack_push(memory, *(stack_el_t *)&f);
}

void inst_icastf(memory_t *memory)
{
  uint64_t value = (uint64_t)stack_pop(memory);
  stack_push(memory, (_Float64)value);
}

void inst_fcasti(memory_t *memory)
{
  uint64_t value = (uint64_t)stack_pop(memory);
  _Float64 f = *(_Float64 *)&value;
  stack_push(memory, f);
}

void inst_cmpeq(memory_t *memory) {}
void inst_icmplt(memory_t *memory) {}
void inst_icmpgt(memory_t *memory) {}
void inst_fcmplt(memory_t *memory) {}
void inst_fcmpgt(memory_t *memory) {}

void inst_neg(memory_t *memory)
{
  stack_el_t a = stack_pop(memory);
  stack_push(memory, ~a);
}

void inst_and(memory_t *memory) {}
void inst_or(memory_t *memory) {}
void inst_xor(memory_t *memory) {}

void inst_load(memory_t *memory)
{
  stack_push(memory, memory->ip->data);
}

void inst_jmp(memory_t *memory) {}
void inst_jmptrue(memory_t *memory) {}
