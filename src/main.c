#include <stdio.h>
#include <stdint.h>

#include "inst.h"
#include "memory.h"

int main()
{
  init_insts();

  inst_t insts[] = {
      {INST_LOAD, 10},
      {INST_LOAD, 32},
      {INST_IADD},
      {INST_HLT},
  };

  memory_t memory = init_memory(insts);
  while (1)
  {
    next_inst(&memory);
  }
}
