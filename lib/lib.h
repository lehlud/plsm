#pragma once

#include <stdint.h>
#include <stdlib.h>

#define _PLSMID(id) __plsm_##id

typedef int64_t _PLSMID(int);
typedef double _PLSMID(float);
typedef char *_PLSMID(string);