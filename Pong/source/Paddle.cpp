#include "Paddle.h"

Paddle::Paddle(int player, float cx, float cy) :GameObject(cx, cy)
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
			setPos(m_c_x, m_c_y - m_speed);
		}
		if (state[SDL_SCANCODE_DOWN]) {
			setPos(m_c_x, m_c_y + m_speed);
		}
		break;
	
	case 2:
		if (state[SDL_SCANCODE_W]) {
			setPos(m_c_x, m_c_y - m_speed);
		}
		if (state[SDL_SCANCODE_S]) {
			setPos(m_c_x, m_c_y + m_speed);
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

	m_c_x = cx;
	m_c_y = cy;
	render_rect.x = cx - m_width / 2;
	render_rect.y = cy - m_height / 2;
}

