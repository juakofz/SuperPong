#pragma once
#include <algorithm>

// ------------------------------------------------- Global objects and constants

//FPS cap
const int g_fps_cap = 60;
const int g_ticks_per_frame = 1000 / g_fps_cap;

//Screen dimension constants
const int g_screen_width = 640;
const int g_screen_height = 480;
const int g_px_size = 5;

//Background square parameters
const int g_margin_x = 5 * g_px_size;
const int g_margin_top = 10 * g_px_size;
const int g_margin_bot = 5 * g_px_size;
const int g_thickness = 1 * g_px_size;

//Game area
const int g_game_area_x = g_screen_width - 2 * (g_margin_x);
const int g_game_area_y = g_screen_height - g_margin_top - g_margin_bot;

//Paddle parameters
const int g_paddle_w = 10;
const int g_paddle_h = 50;
const int g_paddle_margin_x = 50;

//Colors
//const SDL_Color g_color_white = { 0xFF, 0xFF, 0xFF, 0xFF }; //White
//const SDL_Color g_color_black = { 0x00, 0x00, 0x00, 0xFF }; //White

// ------------------------------------------------- Enums

enum en_axis {
    AXIS_X,
    AXIS_Y
};

enum players {
    PLAYER_1,
    PLAYER_2
};

enum alignment {
    TOP,
    BOTTOM,
    LEFT,
    RIGHT,
    CENTER
};

enum corners {
    TOP_RIGHT,
    TOP_LEFT,
    BOTTOM_RIGHT,
    BOTTOM_LEFT
};

// ------------------------------------------------- Useful functions

template <typename T>
T clip(const T & n, const T & lower, const T & upper) {
    return std::max(lower, std::min(n, upper));
}

float adjustSpeed(float s);