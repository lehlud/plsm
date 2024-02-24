#pragma once

#include "type.h"
#include "value.h"

typedef struct
{
  value_t *value;
  list_node_t *next;
} list_node_t;

typedef struct
{
  type_t *type;
  list_node_t *head;
} list_t;
