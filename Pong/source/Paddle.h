#pragma once
#include <iostream>
#include <SDL.h>
#include "Globals.h"
#include "GameObject.h"
#include "Quad.h"

extern SDL_Window* gWindow;

class InteractionObserver;

class Paddle :
	public GameObject
{
	friend class InteractionObserver;

public:

	Paddle();
	Paddle(int player, float cx, float cy);
	
	void render(SDL_Renderer* renderer);
	void processKeys();
	void move(float max_mov);
	
	void setCen(float cx, float cy);
	void setCen(Vector2 pos);
	SDL_Point getSize();
	Quad getQuad();

private:

	//Paddle parameters
	int m_width = 2 * g_px_size;
	int m_height = 10 * g_px_size;
	int m_margin_x;
	int m_margin_y = m_height / 2;
	float m_max_speed = adjustSpeed(300.0f); // in px/second

	//SDL_Color m_color{ 0x00, 0x00, 0xFF, 0xFF }; //Blue for debug
	SDL_Color m_color{ 0xFF, 0xFF, 0xFF, 0xFF }; //White

	int m_player;

	Quad m_object_quad;
};

