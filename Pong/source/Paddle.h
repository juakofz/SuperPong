#pragma once
#include <iostream>
#include "Globals.h"
#include "GameObject.h"
#include <SDL.h>

extern SDL_Window* gWindow;

class Paddle :
	public GameObject
{
public:

	Paddle(int player, float cx, float cy);
	
	void render(SDL_Renderer* renderer);
	void processKeys();
	void move();
	
	void setPos(float cx, float cy);
	void setPos(Vector2 pos);
	SDL_Rect getRect();
	SDL_Point getSize();
	Vector2 getTopRight();
	Vector2 getTopLeft();
	Vector2 getBotRight();
	Vector2 getBotLeft();

private:

	//Paddle parameters
	int m_width = 10;
	int m_height = 50;
	int m_margin_x;
	int m_margin_y = m_height / 2;
	float m_max_speed = adjustSpeed(300.0f); // in px/second

	//SDL_Color m_color{ 0x00, 0x00, 0xFF, 0xFF }; //Blue for debug
	SDL_Color m_color{ 0xFF, 0xFF, 0xFF, 0xFF }; //White

	int m_player;

	SDL_Rect render_rect;
};

