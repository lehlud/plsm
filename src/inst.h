#pragma once

#include <stdint.h>

typedef struct inst_t
{
  uint8_t op;
  uint64_t data;
} inst_t;

#define INST_NOP 0
#define INST_HLT 1

#define INST_IADD 20
#define INST_FADD 40

#define INST_NEG 100

#define INST_LOAD 150

#define INST_JMP 200
#define INST_JMPIF 201

typedef struct memory_t memory_t;
typedef void (*inst_handler_t)(memory_t *memory);

void init_insts();
void next_inst(memory_t *memory);

void inst_nop(memory_t *memory);
void inst_hlt(memory_t *memory);

void inst_iadd(memory_t *memory);
void inst_fadd(memory_t *memory);

void inst_neg(memory_t *memory);

void inst_load(memory_t *memory);

void inst_jmp(memory_t *memory);
void inst_jmpif(memory_t *memory);
