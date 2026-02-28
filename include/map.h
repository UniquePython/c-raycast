#ifndef MAP_H
#define MAP_H

#include <stddef.h>

typedef enum
{
    TILE_EMPTY = 0,
    TILE_WALL
} TileType;

typedef struct
{
    int rows;
    int cols;
    TileType *data;
} Map;

Map *Map_CreateRandom(int rows, int cols);
void Map_Destroy(Map *map);

TileType Map_Get(const Map *map, int row, int col);
void Map_Set(Map *map, int row, int col, TileType value);

#endif