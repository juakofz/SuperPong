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

	m_vel = dir;
	m_vel = m_vel.norm(m_initial_speed);

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
	//Blue for slow ball, red for fast ball
	float temperature = 2 * (m_vel.mod() / m_initial_speed - 1);

	if (temperature >= 0)
	{
		int color = 255 * (1 - (temperature));
		color = clip(color, 0, 255);
		SDL_SetRenderDrawColor(renderer, m_color.r, color, color, m_color.a);
	}
	else
	{
		int color = 255 * (1 + (temperature));
		color = clip(color, 0, 255);
		SDL_SetRenderDrawColor(renderer, color, m_color.g, m_color.b, m_color.a);
	}

	SDL_RenderFillRect(renderer, &m_object_quad.getRect());
}

void Ball::bounceBorder(Collision* coll)
{
	//Move ball to just before collision
	setCen(m_next_cen - m_vel.norm(0.1f));

	//Reflect the appropriate speed component: r = d - 2(d·n)n
	m_vel = m_vel - coll->normal * 2 * (m_vel * coll->normal);

	//Move remaining distance
	move();
}

void Ball::bouncePaddle(Collision* coll, Vector2 pad_speed)
{
	//Move ball to just before collision
	setCen(m_next_cen - m_vel.norm(0.1f));

	//Change reflection angle depending on distance to center
	//float max_angle_change = -10;
	//Vector2 normal = coll->normal.rotate(max_angle_change * coll->center_distance);

	//Add paddle movement to change a bit the ball direction
	//Vector2 normal = (coll->normal.norm() + pad_speed.norm() * 0.1f).norm();

	//Reflect the appropriate speed component: r = d - 2(d·n)n
	Vector2 normal = coll->normal;
	m_vel = m_vel - normal * 2 * (m_vel * normal);

	//Increase ball speed depending on distance to center
	float speed = m_vel.mod();
	float increase = 0.10;
	float decrease = 0.05;
	float factor = abs(coll->center_distance) * (increase + decrease) + 1 - decrease;
	speed = speed * factor;

	//std::cout << "Increase: " << factor << std::endl;

	//m_max_speed *= m_accel_rate;
	m_vel = m_vel.norm(speed);

	//Move remaining distance
	move();
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

void Ball::reset(float cx, float cy)
{
	setCen(cx, cy);
	m_dist_rem = 0;
	m_max_speed = m_initial_speed;
	m_vel = m_vel.norm(m_initial_speed);
}

bool Ball::calculateQuadCollision(Vector2 v_corner_1, Vector2 v_corner_2, Vector2 v_corner_3, Vector2 v_corner_4, Collision* coll)
{

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
				//Relative distance to face center.  -1 at p1, 1 at p2, 0 at center
				float dist_center_p = 2 * aux_intersect.dist(p1)/p1.dist(p2) - 1;

				//Copy intersection to vector
				v_intersecciones.emplace_back(Collision{ aux_intersect, Vector2::getSegmentNormal(p1, p2), corner_relative, dist_center_p });
			}
		}
	}

	//Collision collision;
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
				*coll = *it;
			}
		}
		
		//Update next center
		m_next_cen = coll->point - coll->corner_relative;

		//Remaining distance to travel after collision
		m_dist_rem -= min_dist;

		return true;
	}
	else
	{
		return false; //No collision
	}

}

void Ball::processKeys()
{
	return;
}