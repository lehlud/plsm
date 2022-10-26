#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <getopt.h>

#include "inst.h"
#include "memory.h"

int compileflag = 0;
char *outfile = NULL, *infile = NULL;

int main(int argc, char **argv)
{
  char c;
  while ((c = getopt(argc, argv, "f:co:")) != -1)
  {
    switch (c)
    {
    case 'f':
      infile = optarg;
      break;
    case 'o':
      outfile = optarg;
      break;
    case 'c':
      compileflag = 1;
      break;
    case '?':
      return 1;
    }
  }

  if (!infile)
  {
    fprintf(stderr, "Option -f is required.\n");
    return 1;
  }

  // open specified file
  FILE *f = fopen(infile, "rb");

  // get file size in bytes
  fseek(f, 0, SEEK_END);
  uint64_t fsize = ftell(f);
  fseek(f, 0, SEEK_SET);

  // read instructions and close file
  inst_t *insts = malloc(fsize);
  fread(insts, fsize, 1, f);
  fclose(f);

  init_insts();

  memory_t memory = init_memory(insts);
  while (1)
  {
    next_inst(&memory);
  }
}
