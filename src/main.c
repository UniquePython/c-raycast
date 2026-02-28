#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "error_handling.h"
#include "macros.h"

// --- MACROS ------------>

// --- DATA TYPES ------------>

// --- PROTOTYPES ------------>

// --- ENTRY POINT ------------>

int main(void)
{
	Check_StatusCode(SDL_Init(SDL_INIT_VIDEO));

	SDL_Window *windowptr = SDL_CreateWindow("Raycasting", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, 0);
	Check_Pointer(windowptr);

	SDL_Surface *surfaceptr = SDL_GetWindowSurface(windowptr);
	Check_Pointer(surfaceptr);

	SDL_Rect rect = {WIN_WIDTH / 2, WIN_HEIGHT / 2, 100, 100};
	SDL_FillRect(surfaceptr, &rect, COLOR_WHITE);
	SDL_UpdateWindowSurface(windowptr);

	SDL_Delay(3000);

	return EXIT_SUCCESS;
}

// --- IMPLEMENTATIONS ------------>
