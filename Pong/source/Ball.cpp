#include "Ball.h"

Ball::Ball() :GameObject()
{
	render_rect.w = m_size;
	render_rect.h = m_size;
	m_dist_rem = m_vel.mod();
	p_score_p1 = NULL;
	p_score_p2 = NULL;
}

Ball::Ball(float cx, float cy, Vector2 dir, int* score_p1, int* score_p2)
{
	render_rect.w = m_size;
	render_rect.h = m_size;
	m_dist_rem = m_vel.mod();

	m_vel.set(dir.x * m_max_speed, dir.y * m_max_speed);

	setPos(cx, cy);

	p_score_p1 = score_p1;
	p_score_p2 = score_p2;
}

void Ball::setPos(float cx, float cy)
{
	m_cen.set(cx, cy);

	render_rect.x = (int) (cx - m_size / 2);
	render_rect.y = (int) (cy - m_size / 2);
}

void Ball::setPos(Vector2 pos)
{
	setPos(pos.x, pos.y);
}

void Ball::render(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a);
	SDL_RenderFillRect(renderer, &render_rect); //Left paddle
}

void Ball::move(Paddle pad1, Paddle pad2, SDL_Rect border)
{
	//Reset travel distance
	m_dist_rem = m_vel.mod();

	//Calculate and move intersections	
	bool f1, f2, f3;
	do {
		f1 = bounceQuad(pad1.getTopLeft(), pad1.getTopRight(), pad1.getBotLeft(), pad1.getBotRight()); //with left paddle
		f2 = bounceQuad(pad2.getTopLeft(), pad2.getTopRight(), pad2.getBotLeft(), pad2.getBotRight()); //with right paddle
		std::vector<Vector2> corners = Vector2::getCorners(border);
		Vector2 collision;
		f3 = bounceQuad(corners[0], corners[1], corners[2], corners[3], &collision); //with borders

		if (f3)
		{
			if (collision.x == g_margin_x) //Left side
			{
				*p_score_p2 += 1;
				setPos( (float)g_screen_width / 2.0, (float)g_screen_height / 2.0);

			}
			if (collision.x == g_screen_width - g_margin_x) //Right side
			{
				*p_score_p1 += 1;
				setPos((float)g_screen_width / 2.0, (float)g_screen_height / 2.0);
			}

		}

	} while (f1 || f2 || f3);

	//Normal movement
	if (m_dist_rem > 0)
	{
		Vector2 new_cen = m_cen + m_vel.norm(m_dist_rem);
		setPos(new_cen);
	}
}

bool Ball::bounceQuad(Vector2 v_top_left, Vector2 v_top_right, Vector2 v_bot_left, Vector2 v_bot_right, Vector2* collision)
{
	//Next center under regualr movement
	m_next_cen = m_cen + m_vel.norm(m_dist_rem);

	struct reflection
	{
		Vector2 point;
		int axis;
	};

	std::vector<reflection> v_intersecciones;
	Vector2 aux_intersect;

	/*
	//Top right line
	Vector2 top_right(m_cen.x - m_size/2, m_cen.y - m_size / 2);
	Vector2 top_right_next(m_next_cen.x - m_size / 2, m_next_cen.y - m_size / 2);
	//Top left line
	Vector2 top_left(m_cen.x + m_size / 2, m_cen.y - m_size / 2);
	Vector2 top_left_next(m_next_cen.x + m_size / 2, m_next_cen.y - m_size / 2);
	//Bottom right line
	Vector2 bottom_right(m_cen.x - m_size / 2, m_cen.y + m_size / 2);
	Vector2 bottom_right_next(m_next_cen.x - m_size / 2, m_next_cen.y + m_size / 2);
	//Bottom left line
	Vector2 bottom_left(m_cen.x + m_size / 2, m_cen.y + m_size / 2);
	Vector2 bottom_left_next(m_next_cen.x + m_size / 2, m_next_cen.y + m_size / 2);
	*/

	//Intersection with right side
	/*if (Vector2::intersectSegments(top_right, top_right_next, v_top_right, v_bot_right, &aux_intersect) ||
		Vector2::intersectSegments(top_left, top_left_next, v_top_right, v_bot_right, &aux_intersect) ||
		Vector2::intersectSegments(bottom_right, bottom_right_next, v_top_right, v_bot_right, &aux_intersect) ||
		Vector2::intersectSegments(bottom_left, bottom_left_next, v_top_right, v_bot_right, &aux_intersect) ) */
	if (Vector2::intersectSegments(m_cen, m_next_cen, v_top_right, v_bot_right, &aux_intersect))
	{
		//Copy intersection to vector
		v_intersecciones.emplace_back(reflection{ aux_intersect , AXIS_X });
	}	
	//Intersection with left side
	if (Vector2::intersectSegments(m_cen, m_next_cen, v_top_left, v_bot_left, &aux_intersect))
	{
		//Copy intersection to vector
		v_intersecciones.emplace_back(reflection{ aux_intersect , AXIS_X });
	}
	//Intersection with top side
	if (Vector2::intersectSegments(m_cen, m_next_cen, v_top_left, v_top_right, &aux_intersect))
	{
		//Copy intersection to vector
		v_intersecciones.emplace_back(reflection{ aux_intersect , AXIS_Y });
	}
	//Intersection with bottom side
	if (Vector2::intersectSegments(m_cen, m_next_cen, v_bot_left, v_bot_right, &aux_intersect))
	{
		//Copy intersection to vector
		v_intersecciones.emplace_back(reflection{ aux_intersect , AXIS_Y });
	}

	Vector2 contact_point;
	int reflection_axis;
	float min_dist = 100000;

	//Use the closest intersection to the ball center
	if (v_intersecciones.size() > 0)
	{
		//get distance
		for (auto it = v_intersecciones.begin(); it != v_intersecciones.end(); it++)
		{
			float dist = it->point.dist(m_cen);
			if (dist < min_dist) //Store point if dist < min
			{
				min_dist = dist;
				contact_point = it->point;
				reflection_axis = it->axis;
			}
		}
	}
	else
	{
		return false; //No collision
	}

	//Remaining distance to travel after collision
	m_dist_rem = m_vel.mod() - min_dist;

	//Reflect the appropriate speed component
	if (reflection_axis == AXIS_X)
	{
		m_vel.x *= -1;
	}
	if (reflection_axis == AXIS_Y)
	{
		m_vel.y *= -1;
	}

	if (collision != NULL)
	{
		*collision = contact_point;
	}

	//Travel remaining distance from the collision point
	setPos(contact_point + m_vel.norm(0.1f));
	return true;

}

void Ball::processKeys()
{
	return;
}