#ifndef MACROS_H
#define MACROS_H

static const int WIN_WIDTH = 900;
static const int WIN_HEIGHT = 600;

static const uint32_t COLOR_WHITE = 0xEEEEEEFF;

static const int NROWS = 5;
static const int NCOLS = 5;

static const int CELL_SIZE = 50;

#define PI 3.14159265358979323846
#define DEG(deg) ((deg) * (PI / 180.0f))

static const float HFOV = DEG(60);
static const float ANGLE = DEG(45);

#endif