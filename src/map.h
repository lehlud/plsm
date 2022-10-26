#pragma once

#include <stdint.h>
#include <stdlib.h>

typedef struct map_entry_t
{
  uint8_t *key;
  uint8_t *value;
} map_entry_t;

typedef struct map_entry_list_t
{
  uint64_t size;
  map_entry_t *entries;
} map_entry_list_t;

typedef struct map_t
{
  uint64_t table_size;
  map_entry_list_t *entry_lists;
} map_t;

inline map_t *map_create(uint64_t table_size)
{
  map_t *map = (map_t *)malloc(sizeof(map_t));

  map->table_size = table_size;
  map->entry_lists = (map_entry_list_t *)malloc(table_size * sizeof(map_entry_list_t));

  for (uint64_t i = 0; i < table_size; i++)
  {
    map_entry_list_t entry_list = {0, (map_entry_t *)malloc(0)};
    map->entry_lists[i] = entry_list;
  }

  return map;
}

inline void map_dispose(map_t *map)
{
  for (uint64_t i = 0; i < map->table_size; i++)
  {
    free(map->entry_lists[i].entries);
  }

  free(map->entry_lists);
  free(map);
}
