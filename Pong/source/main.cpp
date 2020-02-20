#pragma once
#include <iostream>
#include "SDL.h"

// ------------------------------------------------- SDL functions

//Starts up SDL and creates window
bool init();

//Load necessary files
bool loadMedia();

//Shuts down SDL
void close();

// ------------------------------------------------- Global objects and constants

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

// ------------------------------------------------- Game parameters

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
	
	SDL_Rect pad_left =  { paddle_margin_x - (paddle_w / 2),
		                   (SCREEN_HEIGHT / 2) - (paddle_h / 2),
						   paddle_w,
						   paddle_h };

	SDL_Rect pad_right = { SCREEN_WIDTH - paddle_margin_x - (paddle_w / 2),
						   (SCREEN_HEIGHT / 2) - (paddle_h / 2),
						   paddle_w,
						   paddle_h };

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
		const Uint8* state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_UP]) {
			pad_left.y -= paddle_speed;
		}
		if (state[SDL_SCANCODE_DOWN]) {
			pad_left.y += paddle_speed;
		}
		if (state[SDL_SCANCODE_W]) {
			pad_right.y -= paddle_speed;
		}
		if (state[SDL_SCANCODE_S]) {
			pad_right.y += paddle_speed;
		}
		/*if (state[SDL_SCANCODE_LEFT]) {
			bob.x -= speed;
		}
		if (state[SDL_SCANCODE_RIGHT]) {
			bob.x += speed;
		}*/

		//Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(gRenderer);

		//Render paddles filled quad
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderFillRect(gRenderer, &pad_left); //Left paddle
		SDL_RenderFillRect(gRenderer, &pad_right); //Right paddle

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