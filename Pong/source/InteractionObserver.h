#pragma once
#include <SDL.h>
#include "Globals.h"
#include "Ball.h"
#include "Paddle.h"

class InteractionObserver
{
	//allow access to observables
	friend class Ball;
	friend class Paddle;

public:

	InteractionObserver();
	InteractionObserver(int* score_p1, int* score_p2, SDL_Rect* p_walls);	
	~InteractionObserver();

	void add(Ball* ball);
	void add(Paddle* pad);

	void movePaddles();
	void moveBalls();

private:

	//Pointers to game objects
	std::vector<Paddle*> v_paddles;
	std::vector<Ball*> v_balls;
	SDL_Rect* p_walls;

	//Pointer to player score
	int* p_score_p1;
	int* p_score_p2;
};

