#pragma once
#include <SDL.h>
#include <vector>
#include "GameObject.h"
#include "Globals.h"
#include "Quad.h"

class InteractionObserver;

class Ball :
	public GameObject
{
	friend class InteractionObserver;

public:

	Ball();
	Ball(float cx, float cy, Vector2 dir, int* score_p1, int* score_p2);

	void render(SDL_Renderer* renderer);
	void processKeys();
	void setCen(float cx, float cy);
	void setCen(Vector2 pos);
	int getSize();
	bool bounceQuad(Vector2 v1, Vector2 v2, Vector2 v3, Vector2 v4, Vector2* collision = NULL);
	void move();

private:

	float m_max_speed = adjustSpeed(300.0f); // in px/second

	int m_size = 10;

	Quad m_object_quad;

	//SDL_Color m_color{ 0x00, 0x00, 0xFF, 0xFF }; //Blue for debug
	SDL_Color m_color{ 0xFF, 0xFF, 0xFF, 0xFF }; //White

	//Distance remaining for movement
	float m_dist_rem;

};

