#include <SDL.h>
#include "minput.h"

MInput::MInput()
{
}

MInput::~MInput()
{
}

bool MInput::init()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
        return false;

    return true;
}

bool MInput::key(SDL_Scancode k)
{
    const Uint8* keyState = SDL_GetKeyboardState(nullptr);
    return keyState[k] != 0;
}

bool MInput::reacquireInput()
{
    return true;
}