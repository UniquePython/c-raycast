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

	SDL_Window *windowptr = SDL_CreateWindow("Raycasting", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
	Check_Pointer(windowptr);

	SDL_Delay(3000);

	return EXIT_SUCCESS;
}

// --- IMPLEMENTATIONS ------------>
