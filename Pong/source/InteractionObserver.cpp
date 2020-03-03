#include "InteractionObserver.h"

InteractionObserver::InteractionObserver(int* score_p1, int* score_p2, SDL_Rect* walls)
{
	p_score_p1 = score_p1;
	p_score_p2 = score_p2;
	p_walls = walls;
}

InteractionObserver::~InteractionObserver()
{
	v_paddles.empty();
	v_balls.empty();
}

void InteractionObserver::add(Ball* ball)
{
	v_balls.push_back(ball);
}

void InteractionObserver::add(Paddle* pad)
{
	v_paddles.push_back(pad);
}

void InteractionObserver::movePaddles()
{
	if (v_paddles.empty()) return;

	//For each paddle
	for (auto it_pad = v_paddles.begin(); it_pad != v_paddles.end(); it_pad++)
	{
		Paddle* pad = *it_pad;

		if (pad->getVel().mod() == 0) continue;

		float max_movement = pad->m_max_speed;

		//For each ball
		if (!v_balls.empty())
		{
			for (auto it_ball = v_balls.begin(); it_ball != v_balls.end(); it_ball++)
			{
				Ball* ball = *it_ball;

				//Vertical collision with ball
				float margin = (ball->getSize() / 2.0f) * 1.1;

				if (ball->getCen().x >= pad->getQuad().getTopLeft().x - margin &&
					ball->getCen().x <= pad->getQuad().getTopRight().x + margin)
				{
					//Remember y is positive downwards!
					if (pad->getQuad().getTopLeft().y + pad->getVel().y - margin <= ball->getCen().y &&
						pad->getQuad().getBotLeft().y + pad->getVel().y + margin >= ball->getCen().y)
					{
						max_movement = std::min(abs(ball->getCen().y - (pad->getQuad().getTopLeft().y + pad->getVel().y - margin)),
												abs(ball->getCen().y - (pad->getQuad().getBotLeft().y + pad->getVel().y + margin)));
						if (max_movement >= 0.1f) max_movement -= 0.1f;
					}
				}
			}
		}

		//Move
		pad->move(max_movement);
	}
	
}

void InteractionObserver::moveBalls()
{
	if (v_balls.empty()) return;

	//For each ball
	for (auto it_ball = v_balls.begin(); it_ball != v_balls.end(); it_ball++)
	{
		Ball* ball = *it_ball;

		//Reset travel distance
		ball->m_dist_rem = ball->getVel().mod();

		//Calculate and move intersections	
		bool f_pads, f_walls;
		do {
			f_pads = false;
			f_walls = false;

			//Collisions with paddles
			for (auto it_pad = v_paddles.begin(); it_pad != v_paddles.end(); it_pad++)
			{
				//Point order is important as it determines normal orientation. See bounceQuad
				Paddle* pad = *it_pad;

				//Bounce
				f_pads = f_pads || ball->bounceQuad(pad->getQuad().getTopLeft(), pad->getQuad().getBotLeft(), pad->getQuad().getBotRight(), pad->getQuad().getTopRight());
			}
			
			//Collisions with borders
			std::vector<Vector2> corners = Vector2::getCorners(*p_walls);
			Vector2 collision;
			f_walls = ball->bounceQuad(corners[0], corners[1], corners[2], corners[3], &collision); 

			//calculate points
			if (f_walls)
			{
				if (collision.x == g_margin_x) //Left side
				{
					*p_score_p2 += 1;
					ball->setCen((float)g_screen_width / 2.0, (float)g_screen_height / 2.0);

				}
				if (collision.x == g_screen_width - g_margin_x) //Right side
				{
					*p_score_p1 += 1;
					ball->setCen((float)g_screen_width / 2.0, (float)g_screen_height / 2.0);
				}
			}

		} while (f_pads || f_walls);

		//Move remaining distance
		ball->move();

		//Lock ball inside game area
		if (ball->getCen().x < p_walls->x) ball->setCen(p_walls->x + 0.1f, ball->getCen().y);
		if (ball->getCen().x > p_walls->x + p_walls->w) ball->setCen(p_walls->x + p_walls->w - 0.1f, ball->getCen().y);
		if (ball->getCen().y < p_walls->y) ball->setCen(ball->getCen().x, p_walls->y + 0.1f);
		if (ball->getCen().y > p_walls->y + p_walls->h) ball->setCen(ball->getCen().x, p_walls->y + p_walls->h - 0.1f);

	}
}



