#include "Paddle.h"

Paddle::Paddle()
{
	m_player = 0;
	m_margin_x = 0;
	setCen(0, 0);
}

Paddle::Paddle(int player, float cx, float cy)
{
	m_object_quad.setSize(m_width, m_height);

	m_player = player;
	m_margin_x = (int)(cx - g_margin_x);

	setCen(cx, cy);
}

void Paddle::render(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a);
	SDL_RenderFillRect(renderer, &m_object_quad.getRect()); //Left paddle
}

void Paddle::processKeys()
{
	//Key is down
	const Uint8* state = SDL_GetKeyboardState(NULL);
	switch (m_player)
	{
	case PLAYER_1:
		if (state[SDL_SCANCODE_W]) {
			m_vel.y = -m_max_speed;
		}
		if (state[SDL_SCANCODE_S]) {
			m_vel.y = m_max_speed;
		}
		break;
	
	case PLAYER_2:
		if (state[SDL_SCANCODE_UP]) {
			m_vel.y = -m_max_speed;
		}
		if (state[SDL_SCANCODE_DOWN]) {
			m_vel.y = m_max_speed;
		}
		break;
	default:
		break;
	}
	
}

void Paddle::setCen(float cx, float cy)
{
	//Restrict paddle movement
	cy = clip(cy, (float)(g_margin_top + m_margin_y), (float)(g_screen_height - g_margin_bot - m_margin_y) );

	m_cen.set(cx, cy);
	m_object_quad.setCenter(cx, cy);
}

void Paddle::setCen(Vector2 pos)
{
	setCen(pos.x, pos.y);
}

SDL_Point Paddle::getSize()
{
	return SDL_Point{m_width, m_height};
}

Quad Paddle::getQuad()
{
	return m_object_quad;
}

void Paddle::move(float max_mov)
{
	Vector2 new_cen = m_cen + m_vel.norm(max_mov);
	setCen(new_cen); //Move to new center
	m_vel.zero(); //Cancel velocity
}

