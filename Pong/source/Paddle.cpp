#include "Paddle.h"

Paddle::Paddle(int player, float cx, float cy)
{
	m_player = player;

	render_rect.w = m_width;
	render_rect.h = m_height;

	setPos(cx, cy);
}

void Paddle::render(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a);
	SDL_RenderFillRect(renderer, &render_rect); //Left paddle
}

void Paddle::processKeys()
{
	//Key is down
	const Uint8* state = SDL_GetKeyboardState(NULL);
	switch (m_player)
	{
	case 1:
		if (state[SDL_SCANCODE_UP]) {
			m_vel.y = -m_max_speed;
		}
		if (state[SDL_SCANCODE_DOWN]) {
			m_vel.y = m_max_speed;
		}
		break;
	
	case 2:
		if (state[SDL_SCANCODE_W]) {
			m_vel.y = -m_max_speed;
		}
		if (state[SDL_SCANCODE_S]) {
			m_vel.y = m_max_speed;
		}
		break;
	default:
		break;
	}
	
}

void Paddle::setPos(float cx, float cy)
{
	cx = clip(cx, (float)m_margin_x, (float) SCREEN_WIDTH - m_margin_x);
	cy = clip(cy, (float)m_margin_y, (float)SCREEN_HEIGHT - m_margin_y);

	m_cen.set(cx, cy);
	render_rect.x = cx - m_width / 2;
	render_rect.y = cy - m_height / 2;
}

void Paddle::setPos(Vector2 pos)
{
	setPos(pos.x, pos.y);
}

SDL_Rect Paddle::getRect()
{
	return render_rect;
}

SDL_Point Paddle::getSize()
{
	return SDL_Point{m_width, m_height};
}

Vector2 Paddle::getTopRight()
{
	return 	Vector2{ m_cen.x + m_width / 2, m_cen.y - m_height / 2 };
}

Vector2 Paddle::getTopLeft()
{
	return 	Vector2{ m_cen.x - m_width / 2, m_cen.y - m_height / 2 };
}

Vector2 Paddle::getBotRight()
{
	return 	Vector2{ m_cen.x + m_width / 2, m_cen.y + m_height / 2 };
}

Vector2 Paddle::getBotLeft()
{
	return 	Vector2{ m_cen.x - m_width / 2, m_cen.y + m_height / 2 };
}

void Paddle::move()
{
	Vector2 new_pos = m_cen + m_vel;
	setPos(new_pos);
	m_vel.zero();
}

