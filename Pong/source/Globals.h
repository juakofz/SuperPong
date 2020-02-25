#pragma once
#include <algorithm>

// ------------------------------------------------- Global objects and constants

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// ------------------------------------------------- Enums

enum en_axis {
    AXIS_X,
    AXIS_Y
};


// ------------------------------------------------- Useful functions

template <typename T>
T clip(const T & n, const T & lower, const T & upper) {
    return std::max(lower, std::min(n, upper));
}