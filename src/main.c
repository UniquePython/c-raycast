#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

// --- MACROS ------------>

#define WIDTH 900
#define HEIGHT 600

// --- DATA TYPES ------------>

typedef enum
{
	ERROR_INT,
	ERROR_PTR
} ErrorType;

typedef struct
{
	ErrorType type;
	union
	{
		int status_code;
		void *ptr;
	} value;
} Error;

// --- PROTOTYPES ------------>

// --- Error Handling ------>

void Handle_SDL_Error(Error);
static inline void Check_StatusCode(int);
static inline void Check_Pointer(void *);

// --- ENTRY POINT ------------>

int main(void)
{
	Check_StatusCode(SDL_Init(SDL_INIT_VIDEO));

	SDL_Window *window = SDL_CreateWindow("Raycasting", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
	Check_Pointer(window);

	SDL_Delay(3000);

	return EXIT_SUCCESS;
}

// --- IMPLEMENTATIONS ------------>

void Handle_SDL_Error(Error error)
{
	switch (error.type)
	{
	case ERROR_INT:
		if (error.value.status_code < 0)
		{
			fprintf(stderr, "%s\n", SDL_GetError());
			exit(EXIT_FAILURE);
		}
		break;

	case ERROR_PTR:
		if (!error.value.ptr)
		{
			fprintf(stderr, "%s\n", SDL_GetError());
			exit(EXIT_FAILURE);
		}
		break;
	}
}

static inline void Check_StatusCode(int x)
{
	Handle_SDL_Error((Error){ERROR_INT, .value.status_code = x});
}

static inline void Check_Pointer(void *p)
{
	Handle_SDL_Error((Error){ERROR_PTR, .value.ptr = p});
}