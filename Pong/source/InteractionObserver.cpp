#include "InteractionObserver.h"

InteractionObserver::InteractionObserver()
{
	p_score_p1 = NULL;
	p_score_p2 = NULL;
	p_scored_flag = NULL;
	p_walls = NULL;
}

InteractionObserver::InteractionObserver(int* score_p1, int* score_p2, bool* scored_flag, SDL_Rect* walls)
{
	p_score_p1 = score_p1;
	p_score_p2 = score_p2;
	p_scored_flag = scored_flag;
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
				float margin = (ball->getSize() / 2.0f) + 0.1f;

				float quad_left_limit = pad->getQuad().getTopLeft().x - margin;
				float quad_right_limit = pad->getQuad().getTopRight().x + margin;
				float quad_top_limit = pad->getQuad().getTopLeft().y - margin;
				float quad_bottom_limit = pad->getQuad().getBotLeft().y + margin;

				//x axis
				if (ball->getCen().x >= quad_left_limit &&
					ball->getCen().x <= quad_right_limit)
				{
					//y axis - remember y is positive downwards!
					if (ball->getCen().y >= quad_top_limit + pad->getVel().y &&
						ball->getCen().y <= quad_bottom_limit + pad->getVel().y)
					{
						//Min distance
						float dist_top = abs(ball->getCen().y - quad_top_limit);
						float dist_bottom = abs(ball->getCen().y - quad_bottom_limit);
						max_movement = std::min(dist_top, dist_bottom);
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
				Collision collision;
				if (ball->calculateQuadCollision(pad->getQuad().getTopLeft(), pad->getQuad().getBotLeft(), pad->getQuad().getBotRight(), pad->getQuad().getTopRight(), &collision))
				{
					//move
					ball->bouncePaddle(&collision, pad->getVel());

					//update flag
					f_pads = true;
				}
			}
			
			//Collisions with borders
			std::vector<Vector2> corners = Vector2::getCorners(*p_walls);
			Collision collision;
			if (ball->calculateQuadCollision(corners[0], corners[1], corners[2], corners[3], &collision))
			{
				//move
				ball->bounceBorder(&collision);

				//update flag
				f_walls = true;
			}

			//calculate points
			if (f_walls)
			{
				float max_angle = 40;

				if (collision.point.x == g_margin_x) //Left side
				{
					*p_score_p2 += 1;
					*p_scored_flag = true;

					//Random start angle towards rigth
					float max = max_angle;
					float min = - max_angle;
					float angle = min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
					Vector2 vel;
					vel = Vector2::angleToVec2(angle);
					ball->setVel(vel);
					ball->reset((float)g_screen_width / 2.0, (float)g_screen_height / 2.0);
				}
				if (collision.point.x == g_screen_width - g_margin_x) //Right side
				{
					*p_score_p1 += 1;
					*p_scored_flag = true;

					//Random start angle towards left
					float max = 180 + max_angle;
					float min = 180 - max_angle;
					float angle = min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
					Vector2 vel;
					vel = Vector2::angleToVec2(angle);
					ball->setVel(vel);
					ball->reset((float)g_screen_width / 2.0, (float)g_screen_height / 2.0);
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



