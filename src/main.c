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

void Handle_Input(bool *, Player *, Map *);

Player Init_Player(const Map *);

float Cast_Ray(const Map *, float, float, float, bool *);
Uint32 Darken_Color(SDL_PixelFormat *, Uint32, float);
void Render_Minimap(SDL_Surface *, const Map *, Player *, Uint32, Uint32, Uint32);
void Render(SDL_Surface *, const Map *, const Player *, Uint32, Uint32, Uint32, Uint32);

// --- ENTRY POINT ------------>

int main(void)
{
	Check_StatusCode(SDL_Init(SDL_INIT_VIDEO));

	SDL_Window *window_ptr = SDL_CreateWindow("Raycasting", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, 0);
	Check_Pointer(window_ptr);

	SDL_Surface *surface_ptr = SDL_GetWindowSurface(window_ptr);
	Check_Pointer(surface_ptr);

	const char *layout =
		"####################"
		"#..................#"
		"#....###...........#"
		"#....#.............#"
		"#....#.....########"
		"#..............#...#"
		"########.......#...#"
		"#..............#...#"
		"#......#########...#"
		"#..................#"
		"####################";

	// Map *map_ptr = Map_CreateFromString(layout, 11, 20);
	Map *map_ptr = Map_CreateRandom(NROWS, NCOLS);
	Handle_NullPtr(map_ptr, "Could not initialize map.");

	Player player = Init_Player(map_ptr);

	Uint32 mapped_yellow_highlight = SDL_MapRGB(surface_ptr->format, 0xC9, 0xBF, 0xAF);
	Uint32 mapped_yellow_shadow = SDL_MapRGB(surface_ptr->format, 0x64, 0x57, 0x47);
	Uint32 mapped_dark_gray = SDL_MapRGB(surface_ptr->format, 0x11, 0x11, 0x11);
	Uint32 mapped_light_gray = SDL_MapRGB(surface_ptr->format, 0x33, 0x33, 0x33);
	Uint32 mapped_minimap_empty = SDL_MapRGB(surface_ptr->format, 0xFF, 0xFF, 0xFF);
	Uint32 mapped_minimap_wall = SDL_MapRGB(surface_ptr->format, 0x00, 0x00, 0x00);
	Uint32 mapped_minimap_player = SDL_MapRGB(surface_ptr->format, 0xAA, 0x00, 0x00);

	// <!------------ EVENT LOOP ------------!>
	bool running = true;
	while (running)
	{
		Handle_Input(&running, &player, map_ptr);

		Render(surface_ptr, map_ptr, &player, mapped_dark_gray, mapped_light_gray, mapped_yellow_highlight, mapped_yellow_shadow);
		Render_Minimap(surface_ptr, map_ptr, &player, mapped_minimap_wall, mapped_minimap_empty, mapped_minimap_player);
		SDL_UpdateWindowSurface(window_ptr);

		SDL_Delay(10); // 100 FPS
	}

	Map_Destroy(map_ptr);

	return EXIT_SUCCESS;
}

// --- IMPLEMENTATIONS ------------>

void Handle_Input(bool *running, Player *player, Map *map)
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

	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP])
	{
		float new_x = player->x + cosf(player->angle) * MOVE_SPEED;
		float new_y = player->y + sinf(player->angle) * MOVE_SPEED;

		if (new_x >= 0 && new_x < map->cols && new_y >= 0 && new_y < map->rows && Map_Get(map, (int)new_y, (int)new_x) != TILE_WALL)
		{
			player->x = new_x;
			player->y = new_y;
		}
	}
	if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN])
	{
		float new_x = player->x - cosf(player->angle) * MOVE_SPEED;
		float new_y = player->y - sinf(player->angle) * MOVE_SPEED;

		if (new_x >= 0 && new_x < map->cols && new_y >= 0 && new_y < map->rows && Map_Get(map, (int)new_y, (int)new_x) != TILE_WALL)
		{
			player->x = new_x;
			player->y = new_y;
		}
	}
	if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])
		player->angle -= ROT_SPEED;
	if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT])
		player->angle += ROT_SPEED;
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
			p.x = c + 0.5f;
			p.y = r + 0.5f;
			p.angle = ANGLE;
			p.hfov = HFOV;
			return p;
		}
	}

	return p;
}

float Cast_Ray(const Map *map, float px, float py, float angle, bool *hit_xside)
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

	while (true)
	{
		if (side_dist_x < side_dist_y)
		{
			side_dist_x += delta_dist_x;
			map_x += step_x;
			*hit_xside = true;
		}
		else
		{
			side_dist_y += delta_dist_y;
			map_y += step_y;
			*hit_xside = false;
		}

		if (map_x < 0 || map_x >= map->cols || map_y < 0 || map_y >= map->rows)
			return INFINITY;

		if (Map_Get(map, map_y, map_x) == TILE_WALL)
			break;
	}

	return *hit_xside ? side_dist_x - delta_dist_x : side_dist_y - delta_dist_y;
}

Uint32 Darken_Color(SDL_PixelFormat *fmt, Uint32 color, float distance)
{
	Uint8 r = (color >> fmt->Rshift) & 0xFF;
	Uint8 g = (color >> fmt->Gshift) & 0xFF;
	Uint8 b = (color >> fmt->Bshift) & 0xFF;

	r = (Uint8)(r * distance);
	g = (Uint8)(g * distance);
	b = (Uint8)(b * distance);

	return SDL_MapRGB(fmt, r, g, b);
}

void Render_Minimap(SDL_Surface *surface, const Map *map, Player *player, Uint32 col_wall, Uint32 col_empty, Uint32 col_player)
{
	for (int r = 0; r < map->rows; r++)
	{
		for (int c = 0; c < map->cols; c++)
		{
			SDL_Rect tile = {MINIMAP_X + c * MINIMAP_TILE_SIZE, MINIMAP_Y + r * MINIMAP_TILE_SIZE, MINIMAP_TILE_SIZE, MINIMAP_TILE_SIZE};
			Uint32 color = Map_Get(map, r, c) == TILE_WALL ? col_wall : col_empty;

			SDL_FillRect(surface, &tile, color);
		}
	}

	int px = MINIMAP_X + player->x * MINIMAP_TILE_SIZE;
	int py = MINIMAP_Y + player->y * MINIMAP_TILE_SIZE;
	SDL_Rect player_rect = {px, py, MINIMAP_TILE_SIZE, MINIMAP_TILE_SIZE};

	SDL_FillRect(surface, &player_rect, col_player);

	for (int t = 0; t < MINIMAP_RAY_LEN; t++)
	{
		int dx = MINIMAP_X + (player->x + cosf(player->angle) * t) * MINIMAP_TILE_SIZE;
		int dy = MINIMAP_Y + (player->y + sinf(player->angle) * t) * MINIMAP_TILE_SIZE;

		SDL_Rect point = {dx, dy, 3, 3};

		SDL_FillRect(surface, &point, col_player);
	}
}

void Render(SDL_Surface *surface, const Map *map, const Player *player, Uint32 col_ceil, Uint32 col_floor, Uint32 col_wall_highlight, Uint32 col_wall_shadow)
{
	SDL_Rect top_half = {0, 0, WIN_WIDTH, WIN_HEIGHT / 2};
	SDL_FillRect(surface, &top_half, col_ceil);

	SDL_Rect bottom_half = {0, WIN_HEIGHT / 2, WIN_WIDTH, WIN_HEIGHT / 2};
	SDL_FillRect(surface, &bottom_half, col_floor);

	bool hitx;

	for (int col = 0; col < WIN_WIDTH; col++)
	{
		hitx = false;
		float ray_angle = (player->angle - player->hfov) + 2 * ((float)col / WIN_WIDTH) * player->hfov;
		float distance = Cast_Ray(map, player->x, player->y, ray_angle, &hitx);
		float corrected = distance * cosf(ray_angle - player->angle);
		int wall_height = (int)(WIN_HEIGHT / corrected);

		SDL_Rect stripe = {col, (WIN_HEIGHT - wall_height) / 2, 1, wall_height};
		float t = 1.0 / (1.0 + corrected * FOG_FACTOR);
		if (hitx)
			SDL_FillRect(surface, &stripe, Darken_Color(surface->format, col_wall_highlight, t));
		else
			SDL_FillRect(surface, &stripe, Darken_Color(surface->format, col_wall_shadow, t));
	}
}
