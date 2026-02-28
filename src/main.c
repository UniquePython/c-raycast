#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>

#include "error_handling.h"
#include "constants.h"
#include "map.h"

// --- MACROS ------------>

// --- DATA TYPES ------------>

typedef struct
{
	float x, y, hfov, angle;
} Player;

// --- PROTOTYPES ------------>

void Handle_Input(bool *);

Player Init_Player(const Map *);

float Cast_Ray(const Map *, float, float, float);

// --- ENTRY POINT ------------>

int main(void)
{
	Check_StatusCode(SDL_Init(SDL_INIT_VIDEO));

	SDL_Window *window_ptr = SDL_CreateWindow("Raycasting", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, 0);
	Check_Pointer(window_ptr);

	SDL_Surface *surface_ptr = SDL_GetWindowSurface(window_ptr);
	Check_Pointer(surface_ptr);

	Map *map_ptr = Map_CreateRandom(NROWS, NCOLS);
	Handle_NullPtr(map_ptr, "Could not initialize map.");

	// Map_Print(map_ptr);

	Player player = Init_Player(map_ptr);
	// printf("x: %g\n", player.x);
	// printf("y: %g\n", player.y);

	// <!------------ EVENT LOOP ------------!>
	bool running = true;
	while (running)
	{
		Handle_Input(&running);

		SDL_Rect rect = {WIN_WIDTH / 2, WIN_HEIGHT / 2, 100, 100};
		SDL_FillRect(surface_ptr, &rect, COLOR_WHITE);
		SDL_UpdateWindowSurface(window_ptr);
		SDL_Delay(10); // 100 FPS
	}

	Map_Destroy(map_ptr);

	return EXIT_SUCCESS;
}

// --- IMPLEMENTATIONS ------------>

void Handle_Input(bool *running)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			*running = false;
			break;
		}
	}
}

Player Init_Player(const Map *map)
{
	Player p = {0};

	if (!map || !map->data)
		return p;

	int r, c;
	int max_attempts = map->rows * map->cols;

	while (max_attempts--)
	{
		r = rand() % map->rows;
		c = rand() % map->cols;

		if (Map_Get(map, r, c) == TILE_EMPTY)
		{
			p.x = c;
			p.y = r;
			p.angle = ANGLE;
			p.hfov = HFOV;
			return p;
		}
	}

	return p;
}

float Cast_Ray(const Map *map, float px, float py, float angle)
{
	float ray_dir_x = cosf(angle);
	float ray_dir_y = sinf(angle);
	float delta_dist_x = fabsf(1 / ray_dir_x);
	float delta_dist_y = fabsf(1 / ray_dir_y);
	int map_x = (int)px;
	int map_y = (int)py;
	int step_x = ray_dir_x > 0 ? 1 : -1;
	int step_y = ray_dir_y > 0 ? 1 : -1;
	float side_dist_x = step_x > 0 ? delta_dist_x * (ceil(px) - px) : delta_dist_x * (px - floor(px));
	float side_dist_y = step_y > 0 ? delta_dist_y * (ceil(py) - py) : delta_dist_y * (py - floor(py));
	bool hitx = false;

	while (true)
	{
		if (side_dist_x < side_dist_y)
		{
			side_dist_x += delta_dist_x;
			map_x += step_x;
			hitx = true;
		}
		else
		{
			side_dist_y += delta_dist_y;
			map_y += step_y;
		}

		if (map_x < 0 || map_x >= map->cols || map_y < 0 || map_y >= map->rows)
			return INFINITY;

		if (Map_Get(map, map_y, map_x) == TILE_WALL)
			break;
	}

	return hitx ? side_dist_x - delta_dist_x : side_dist_y - delta_dist_y;
}
