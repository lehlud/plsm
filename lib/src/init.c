#include "init.h"

#include <gc.h>

void plsm_init()
{
  static int initialized = 0;

  if (initialized)
    return;

  GC_INIT();

  initialized = 1;
}
