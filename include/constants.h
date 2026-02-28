#ifndef CONSTANTS_H
#define CONSTANTS_H

static const int WIN_WIDTH = 900;
static const int WIN_HEIGHT = 600;

static const int MINIMAP_X = 15;
static const int MINIMAP_Y = 15;
static const int MINIMAP_TILE_SIZE = 7;
static const int MINIMAP_RAY_LEN = 3;

static const int NROWS = 21;
static const int NCOLS = 21;

#define PI 3.14159265358979323846
#define DEG(deg) ((deg) * (PI / 180.0f))

static const float HFOV = DEG(20);
static const float ANGLE = DEG(45);

static const float FOG_FACTOR = 2.0f;

static const float MOVE_SPEED = 0.03f;
static const float ROT_SPEED = 0.02f;

#endif