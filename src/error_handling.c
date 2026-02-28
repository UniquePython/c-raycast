#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "error_handling.h"

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

void Handle_NullPtr(void *p, const char *msg)
{
    if (!p)
    {
        fprintf(stderr, msg);
        exit(EXIT_FAILURE);
    }
}