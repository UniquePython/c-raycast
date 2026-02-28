#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

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

void Handle_SDL_Error(Error);

static inline void Check_StatusCode(int x)
{
    Handle_SDL_Error((Error){ERROR_INT, .value.status_code = x});
}

static inline void Check_Pointer(void *p)
{
    Handle_SDL_Error((Error){ERROR_PTR, .value.ptr = p});
}

#endif