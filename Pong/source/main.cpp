#pragma once
#include <iostream>
#include <SDL.h>
#include "Globals.h"
#include "Paddle.h"

// ------------------------------------------------- Global objects

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

// ------------------------------------------------- SDL functions

//Starts up SDL and creates window
bool init();

//Load necessary files
bool loadMedia();

//Shuts down SDL
void close();

// ------------------------------------------------- Game parameters

//Background square parameters
int bg_margin_x = 20;
int bg_margin_y = 20;
int bg_thichness = 5;

//Paddle parameters
int paddle_w = 10;
int paddle_h = 50;
int paddle_margin_x = 50;
int paddle_margin_y = 50;
float paddle_speed = 1;

// ------------------------------------------------- Main loop

int main(int argc, char* args[])
{
	if (!init())
	{
		printf("Failed to initialise SDL!");
		return -1;
	}

	if (!loadMedia())
	{
		printf("Failed to load media!");
		return -1;
	}
	//Main loop flag
	bool f_quit = false;

	//Event handler
	SDL_Event e;

	//Paddles
	Paddle pad_left(1, (float)paddle_margin_x, (float) SCREEN_HEIGHT / 2.0);
	Paddle pad_right(2, (float) (SCREEN_WIDTH - paddle_margin_x), (float) SCREEN_HEIGHT / 2.0);

	//Background rectangle
	SDL_Rect bg_white{ bg_margin_x, bg_margin_y,
					   SCREEN_WIDTH - 2 * bg_margin_x, SCREEN_HEIGHT - 2 * bg_margin_y };
	SDL_Rect bg_black{ bg_margin_x + bg_thichness, bg_margin_y + bg_thichness,
					   SCREEN_WIDTH - 2 * (bg_margin_x + bg_thichness), SCREEN_HEIGHT - 2 * (bg_margin_y + bg_thichness) };

	//While application is running
	while (!f_quit)
	{
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				printf("Window was closed");
				f_quit = true;
			}

			//User presses a key
			else if (e.type == SDL_KEYDOWN)
			{
				//switch key presses
				switch (e.key.keysym.sym)
				{
				case SDLK_q:
					printf("Quit key pressed");
					f_quit = true;
					break;
				}
			}
		}

		//Key is down
		pad_left.processKeys();
		pad_right.processKeys();

		/*
		if (state[SDL_SCANCODE_W]) {
			pad_right.y -= paddle_speed;
		}
		if (state[SDL_SCANCODE_S]) {
			pad_right.y += paddle_speed;
		}
		if (state[SDL_SCANCODE_LEFT]) {
			bob.x -= speed;
		}
		if (state[SDL_SCANCODE_RIGHT]) {
			bob.x += speed;
		}*/

		//Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(gRenderer);

		//Render background rectangles
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderFillRect(gRenderer, &bg_white); //white line

		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderFillRect(gRenderer, &bg_black); //black interior


		//Render paddles filled quad
		pad_left.render(gRenderer);
		pad_right.render(gRenderer);

		//Update the surface
		SDL_UpdateWindowSurface(gWindow);

		//Update screen
		SDL_RenderPresent(gRenderer);

	}

	close();

	return 0;

}

bool init()
{

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	//Create window
	gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	//Create renderer for window
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gRenderer == NULL)
	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	return true;
}

bool loadMedia()
{
	//Success flag
	bool success = true;

	//Nothing to load
	return success;
}

void close()
{
	//Destroy window
	SDL_DestroyWindow(gWindow);

	//Quit SDL subsystems
	SDL_Quit();
}