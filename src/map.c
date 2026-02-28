#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "map.h"

static void Carve(Map *map, int r, int c)
{
    int dirs[4][2] = {{0, 2}, {0, -2}, {2, 0}, {-2, 0}};

    // shuffle dirs
    for (int i = 3; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int tr = dirs[i][0], tc = dirs[i][1];
        dirs[i][0] = dirs[j][0];
        dirs[i][1] = dirs[j][1];
        dirs[j][0] = tr;
        dirs[j][1] = tc;
    }

    for (int i = 0; i < 4; i++)
    {
        int nr = r + dirs[i][0];
        int nc = c + dirs[i][1];

        if (nr <= 0 || nr >= map->rows - 1 || nc <= 0 || nc >= map->cols - 1)
            continue;

        if (Map_Get(map, nr, nc) == TILE_WALL)
        {
            // carve wall between
            Map_Set(map, r + dirs[i][0] / 2, c + dirs[i][1] / 2, TILE_EMPTY);
            Map_Set(map, nr, nc, TILE_EMPTY);
            Carve(map, nr, nc);
        }
    }
}

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

    // fill all walls
    for (int i = 0; i < rows * cols; i++)
        map->data[i] = TILE_WALL;

    // start carving from (1,1)
    Map_Set(map, 1, 1, TILE_EMPTY);
    Carve(map, 1, 1);

    return map;
}

Map *Map_CreateFromString(const char *layout, int rows, int cols)
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

    for (int i = 0; i < rows * cols; i++)
        map->data[i] = (layout[i] == '#') ? TILE_WALL : TILE_EMPTY;

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
