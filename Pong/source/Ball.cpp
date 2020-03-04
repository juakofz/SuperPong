#include "Ball.h"

Ball::Ball() :GameObject()
{
	m_object_quad.setSize((float)m_size, (float)m_size);
	m_dist_rem = m_vel.mod();
}

Ball::Ball(float cx, float cy, Vector2 dir, int* score_p1, int* score_p2)
{
	m_object_quad.setSize((float)m_size, (float)m_size);
	m_dist_rem = m_vel.mod();

	m_vel.set(dir.x * m_max_speed, dir.y * m_max_speed);

	setCen(cx, cy);
}

void Ball::setCen(float cx, float cy)
{
	m_cen.set(cx, cy);

	m_object_quad.setCenter(cx, cy);
}

void Ball::setCen(Vector2 pos)
{
	setCen(pos.x, pos.y);
}

int Ball::getSize()
{
	return m_size;
}

void Ball::render(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a);
	SDL_RenderFillRect(renderer, &m_object_quad.getRect()); //Left paddle
	
	/*
	//Visual debug info
	SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
	std::vector<Vector2> v_ball_corners = m_object_quad.getCorners();
	for (int i = 0; i < 4; i++)
	{
		Vector2 ball_corner = v_ball_corners.at(i);
		Vector2 ball_corner_next = ball_corner + m_vel.norm(m_dist_rem);
		SDL_RenderDrawLine(renderer, (int)ball_corner.x, (int)ball_corner.y, (int)ball_corner_next.x, (int)ball_corner_next.y);
	}
	*/
}

void Ball::move()
{
	//Normal movement
	if (m_dist_rem > 0)
	{
		Vector2 new_cen = m_cen + m_vel.norm(m_dist_rem);
		setCen(new_cen);
	}
}

bool Ball::bounceQuad(Vector2 v_corner_1, Vector2 v_corner_2, Vector2 v_corner_3, Vector2 v_corner_4, Vector2* coll_point)
{

	struct Collision
	{
		Vector2 point;
		Vector2 normal;
		Vector2 corner_relative;
	};

	//Paddle or wall corners
	std::vector<Vector2> target_corners;
	target_corners.push_back(v_corner_1);
	target_corners.push_back(v_corner_2);
	target_corners.push_back(v_corner_3);
	target_corners.push_back(v_corner_4);

	//Ball corners
	std::vector<Vector2> v_ball_corners = m_object_quad.getCorners();

	std::vector<Collision> v_intersecciones;
	Vector2 aux_intersect;
	
	//Check for corner - corner collision
	for (int i = 0; i < 4; i++)
	{
		Vector2 ball_corner = v_ball_corners.at(i);
		Vector2 ball_corner_next = ball_corner + m_vel.norm(m_dist_rem);
		
		for (int j = 0; j < 4; j++)
		{
			Vector2 target_corner = target_corners.at(j);

			//If corner collision move a bit
			if (Vector2::isPointInSegment(ball_corner, ball_corner_next, target_corner))
			{
				setCen(m_cen.x - m_vel.x*0.1, m_cen.y);
			}

		}
	}

	//For every ball corner
	for (int i = 0; i < 4; i++)
	{
		Vector2 ball_corner = v_ball_corners.at(i);
		Vector2 ball_corner_next = ball_corner + m_vel.norm(m_dist_rem);
		Vector2 corner_relative = ball_corner - m_cen;

		//Calculate intersections, taking points in pairs to form segments.
		//Take every corner-side pair
		//Segment normal is calculated clockwise. When points are passed clockise, normals point inwards, and vice versa.
		for (int j = 0; j < 4; j++)
		{
			Vector2 p1 = target_corners.at(j);
			Vector2 p2;
			if (j < 3) p2 = target_corners.at(j + 1);
			else p2 = target_corners.at(0); //Pair last to first

			//Intersection with side
			if (Vector2::intersectSegments(ball_corner, ball_corner_next, p1, p2, &aux_intersect))
			{
				//Copy intersection to vector
				v_intersecciones.emplace_back(Collision{ aux_intersect, Vector2::getSegmentNormal(p1, p2), corner_relative });
			}
		}
	}

	Collision collision;
	float min_dist = 100000;

	//Use the closest intersection to the ball center
	if (v_intersecciones.size() > 0)
	{
		//get distance
		for (auto it = v_intersecciones.begin(); it != v_intersecciones.end(); it++)
		{
			float dist = it->point.dist(m_cen + it->corner_relative);
			if (dist < min_dist) //Store point if distance from corner < min
			{
				min_dist = dist; //Calculate distance from corner
				collision = *it;
			}
		}
	}
	else
	{
		return false; //No collision
	}

	m_next_cen = collision.point - collision.corner_relative;

	//Remaining distance to travel after collision
	m_dist_rem -= min_dist;

	//Move ball to just before collision
	setCen(m_next_cen - m_vel.norm(0.1f));

	//Reflect the appropriate speed component: r = d - 2(d·n)n
	m_vel = m_vel - collision.normal * 2 * (m_vel * collision.normal);

	if (coll_point != NULL)
	{
		*coll_point = collision.point;
	}

	//Move remaining distance
	move();
	return true;

}

void Ball::processKeys()
{
	return;
}