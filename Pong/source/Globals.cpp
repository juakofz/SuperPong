#include "Globals.h"

float adjustSpeed(float s)
{
    return s / g_fps_cap;
}