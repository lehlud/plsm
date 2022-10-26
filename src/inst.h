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
#define INST_FADD 50

#define INST_ICASTF 90
#define INST_FCASTI 91

#define INST_CMPEQ 120
#define INST_ICMPLT 130
#define INST_ICMPGT 131
#define INST_FCMPLT 140
#define INST_FCMPGT 141

#define INST_NEG 150
#define INST_AND 170
#define INST_OR 171
#define INST_XOR 172

#define INST_LOAD 200

#define INST_JMP 220
#define INST_JMPTRUE 221

typedef struct memory_t memory_t;
typedef void (*inst_handler_t)(memory_t *memory);

void init_insts();
void next_inst(memory_t *memory);

void inst_nop(memory_t *memory);
void inst_hlt(memory_t *memory);

void inst_iadd(memory_t *memory);
void inst_fadd(memory_t *memory);

void inst_icastf(memory_t *memory);
void inst_fcasti(memory_t *memory);

void inst_cmpeq(memory_t *memory);
void inst_icmplt(memory_t *memory);
void inst_icmpgt(memory_t *memory);
void inst_fcmplt(memory_t *memory);
void inst_fcmpgt(memory_t *memory);

void inst_neg(memory_t *memory);
void inst_and(memory_t *memory);
void inst_or(memory_t *memory);
void inst_xor(memory_t *memory);

void inst_load(memory_t *memory);

void inst_jmp(memory_t *memory);
void inst_jmptrue(memory_t *memory);
