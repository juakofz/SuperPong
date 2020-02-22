#pragma once
#include <iostream>
#include "Globals.h"
#include "GameObject.h"
#include <SDL.h>

extern SDL_Window* gWindow;

//i am small change 

class Paddle :
	public GameObject
{
public:

	Paddle(int player, float cx, float cy);
	void render(SDL_Renderer* renderer);
	void processKeys();
	void setPos(float cx, float cy);

private:

	//Paddle parameters
	int m_width = 10;
	int m_height = 50;
	int m_margin_x = 50;
	int m_margin_y = 50;
	float m_speed = 0.5;

	//SDL_Color m_color{ 0x00, 0x00, 0xFF, 0xFF }; //Blue for debug
	SDL_Color m_color{ 0xFF, 0xFF, 0xFF, 0xFF }; //White

	int m_player;

	SDL_Rect render_rect;
};

