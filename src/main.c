#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

#include "error_handling.h"
#include "constants.h"
#include "map.h"

// --- MACROS ------------>

// --- DATA TYPES ------------>

// --- PROTOTYPES ------------>

void Handle_Input(bool *);

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
