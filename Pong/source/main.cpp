#pragma once
#include <iostream>
#include <SDL.h>
#include "Globals.h"
#include "Text.h"
#include "Timer.h"
#include "Paddle.h"
#include "Ball.h"
#include "DashedLine.h"
#include "InteractionObserver.h"


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


//--------------------------------------------------------------------- Main

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

	//------------------------------------------------------------------------- Game objects and variables


	//Player score
	int score_p1{ 0 }, score_p2{ 0 };

	//Paddles
	Paddle pad_left(PLAYER_1, (float)g_paddle_margin_x, (float) (g_margin_top + g_game_area_y / 2) );
	Paddle pad_right(PLAYER_2, (float) (g_screen_width - g_paddle_margin_x), (float)(g_margin_top + g_game_area_y / 2) );

	//Ball
	Ball ball((float)g_screen_width / 2.0, (float)g_screen_height / 2.0, Vector2{ 1, -1 }, &score_p1, &score_p2 );

	//Background rectangle. Black rect follows interior perimeter (game area)
	SDL_Rect bg_white{ g_margin_x - g_thickness, g_margin_top - g_thickness,
					   g_game_area_x + 2 * g_thickness , g_game_area_y + 2 * g_thickness };
	SDL_Rect bg_black{ g_margin_x, g_margin_top,
					   g_game_area_x, g_game_area_y };

	//Interaction observer
	InteractionObserver observer(&score_p1, &score_p2, &bg_black);
	observer.add(&pad_left);
	observer.add(&pad_right);
	observer.add(&ball);

	//Center dashed line
	DashedLine center_line(	g_margin_x + g_game_area_x/2, g_margin_top, 
							g_px_size, g_game_area_y, 2 * g_px_size);

	//Text
	Text text_score_p1(gRenderer);
	Text text_score_p2(gRenderer);
	std::string font = "font/visitor1.ttf";
	//std::string font = "font/Gamer.ttf";
	int score_size = 80;

	SDL_Color color_white{ 0xFF, 0xFF, 0xFF, 0xFF }; //White
	SDL_Color color_black{ 0x00, 0x00, 0x00, 0xFF }; //White

	//FPS cap variables
	Timer fps_timer, cap_timer;
	int countedFrames = 0;
	fps_timer.start();

	//------------------------------------------------------------------------- Main loop

	//While application is running
	while (!f_quit)
	{
		cap_timer.start(); //FPS cap

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

		//--------------------------------------------------------------------- FPS counter

		//Calculate de fps
		float avgFPS = countedFrames / (fps_timer.getTicks() / 1000.f);
		if (avgFPS > 2000000) avgFPS = 0;
		if (fps_timer.getTicks() > 2000)
		{
			countedFrames = 0;
			fps_timer.start();
		}
		//std::cout << "FPS: " << avgFPS << std::endl;

		//--------------------------------------------------------------------- Game

		//Key is down
		pad_left.processKeys();
		pad_right.processKeys();

		//Move
		observer.movePaddles();
		observer.moveBalls();

		//Update score and text

		text_score_p1.free();
		text_score_p1.loadText(std::to_string(score_p1), font, score_size, color_white);

		text_score_p2.free();
		text_score_p2.loadText(std::to_string(score_p2), font, score_size, color_white);


		//--------------------------------------------------------------------- Rendering

		//Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(gRenderer);

		//Render background rectangles
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderFillRect(gRenderer, &bg_white); //white line

		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderFillRect(gRenderer, &bg_black); //black interior

		//Render center lines
		center_line.render(gRenderer);

		//Render score text
		text_score_p1.render(gRenderer, g_screen_width/2 - 10, g_margin_top, RIGHT);
		text_score_p2.render(gRenderer, g_screen_width/2 + 22, g_margin_top, LEFT);

		//Render paddles and ball
		pad_left.render(gRenderer);
		pad_right.render(gRenderer);
		ball.render(gRenderer);

		//Update the surface
		SDL_UpdateWindowSurface(gWindow);

		//Update screen
		SDL_RenderPresent(gRenderer);

		//--------------------------------------------------------------------- FPS cap

		++countedFrames;
		int frameTicks = cap_timer.getTicks();
		if (frameTicks < g_ticks_per_frame) //If frame finished early (cap)
		{
			//Wait remaining time
			SDL_Delay(g_ticks_per_frame - frameTicks);
		}

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

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return false;
	}

	//Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		return false;
	}

	//Create window
	gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, g_screen_width, g_screen_height, SDL_WINDOW_SHOWN);
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