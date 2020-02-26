#pragma once
#include "GameObject.h"
#include "Globals.h"
#include "Paddle.h"
#include <SDL.h>
#include <vector>

class Ball :
	public GameObject
{
public:

	Ball();
	Ball(float cx, float cy, Vector2 dir, int* score_p1, int* score_p2);

	void render(SDL_Renderer* renderer);
	void processKeys();
	void setPos(float cx, float cy);
	void setPos(Vector2 pos);
	bool bounceQuad(Vector2 v1, Vector2 v2, Vector2 v3, Vector2 v4, Vector2* collision = NULL);
	void move(Paddle pad1, Paddle pad2, SDL_Rect border);

private:

	float m_max_speed = adjustSpeed(300.0f); // in px/second

	int m_size = 10;
	SDL_Rect render_rect;

	//SDL_Color m_color{ 0x00, 0x00, 0xFF, 0xFF }; //Blue for debug
	SDL_Color m_color{ 0xFF, 0xFF, 0xFF, 0xFF }; //White

	//Distance remaining for movement
	float m_dist_rem;

	//Pointer to player score
	int* p_score_p1;
	int* p_score_p2;

};

