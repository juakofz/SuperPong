#pragma once
#include <algorithm>

// ------------------------------------------------- Global objects and constants

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// ------------------------------------------------- Useful functions

template <typename T>
T clip(const T & n, const T & lower, const T & upper) {
    return std::max(lower, std::min(n, upper));
}