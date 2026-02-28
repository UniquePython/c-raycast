#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "map.h"

Map *Map_CreateRandom(int rows, int cols)
{
    Map *map = malloc(sizeof *map);
    if (!map)
        return NULL;

    map->rows = rows;
    map->cols = cols;

    map->data = malloc(rows * cols * sizeof *map->data);
    if (!map->data)
    {
        free(map);
        return NULL;
    }

    srand(time(NULL));

    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            map->data[r * cols + c] = (TileType)(rand() % N_TILES);

    return map;
}

void Map_Destroy(Map *map)
{
    if (!map)
        return;
    free(map->data);
    free(map);
}

TileType Map_Get(const Map *map, int row, int col)
{
    assert(map);
    assert(row >= 0 && row < map->rows);
    assert(col >= 0 && col < map->cols);

    return map->data[row * map->cols + col];
}

void Map_Set(Map *map, int row, int col, TileType value)
{
    assert(map);
    assert(row >= 0 && row < map->rows);
    assert(col >= 0 && col < map->cols);

    map->data[row * map->cols + col] = value;
}

void Map_Print(const Map *map)
{
    if (!map || !map->data)
    {
        printf("(null map)\n");
        return;
    }

    for (int r = 0; r < map->rows; r++)
    {
        for (int c = 0; c < map->cols; c++)
        {
            TileType t = Map_Get(map, r, c);

            switch (t)
            {
            case TILE_WALL:
                putchar('#');
                break;
            case TILE_EMPTY:
            default:
                putchar('.');
                break;
            }
        }
        putchar('\n');
    }
}
