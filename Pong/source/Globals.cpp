#include "Globals.h"

float adjustSpeed(float s)
{
    return s / g_fps_cap;
}

void setRenderDrawColorWhite(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

void setRenderDrawColorBlack(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}
