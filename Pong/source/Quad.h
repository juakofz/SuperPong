#pragma once
#include <SDL.h>
#include <vector>
#include "Vector2.h"

class Quad
{
public:	

	Quad();
	Quad(SDL_Rect r);
	Quad(int x, int y, int w, int h);
	Quad(float x, float y, float w, float h);

	//Get center
	Vector2 getCen();
	
	//Get corners
	std::vector<Vector2> getCorners();
	Vector2 getTopRight();
	Vector2 getTopLeft();
	Vector2 getBotRight();
	Vector2 getBotLeft();

	//Get rect
	SDL_Rect getRect();

	//Set center
	void setCenter(float x, float y);
	void setTopLeft(float x, float y);
	void setSize(float w, float h);

private:
	SDL_Rect m_rect;
	Vector2 m_cen, m_pos;
	Vector2 m_size;
};

