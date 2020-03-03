#include "Quad.h"

Quad::Quad()
{
	//Set everything to zero 
	m_rect.x = 0;
	m_rect.y = 0;
	m_rect.w = 0;
	m_rect.h = 0;

	m_cen.zero();
	m_pos.zero();
}

Quad::Quad(SDL_Rect r)
{
	//Set SDL_Rect
	m_rect.x = r.x;
	m_rect.y = r.y;
	m_rect.w = r.w;
	m_rect.h = r.h;

	//Set floats
	m_pos = Vector2(r.x, r.y);
	m_cen = Vector2(r.x + r.w / 2, r.y + r.h / 2);
}

Quad::Quad(int x, int y, int w, int h)
{
	m_rect.x = x;
	m_rect.y = y;
	m_rect.w = w;
	m_rect.h = h;

	m_pos = Vector2((float)x, (float)y);
	m_cen = Vector2((float)(x + w / 2.0f), (float)(y + h / 2.0f));
}

Quad::Quad(float x, float y, float w, float h)
{
	m_rect.x = (int)x;
	m_rect.y = (int)y;
	m_rect.w = (int)w;
	m_rect.h = (int)h;

	m_pos = Vector2(x, y);
	m_cen = Vector2(x + w / 2.0f, y + h / 2.0f);
}

Vector2 Quad::getCen()
{
	return m_cen;
}

std::vector<Vector2> Quad::getCorners()
{
	//Returns 4 corners, from top left clockwise
	std::vector<Vector2> aux;
	aux.push_back(getTopLeft());
	aux.push_back(getTopRight());
	aux.push_back(getBotRight());
	aux.push_back(getBotLeft());

	return aux;
}

Vector2 Quad::getTopRight()
{
	return Vector2{ m_pos.x + m_size.x, m_pos.y };
}

Vector2 Quad::getTopLeft()
{
	return 	m_pos;
}

Vector2 Quad::getBotRight()
{
	return 	Vector2{ m_pos.x + m_size.x, m_pos.y + m_size.y };
}

Vector2 Quad::getBotLeft()
{
	return 	Vector2{ m_pos.x, m_pos.y + m_size.y };
}

SDL_Rect Quad::getRect()
{
	return m_rect;
}

void Quad::setCenter(float x, float y)
{
	m_cen.x = x;
	m_cen.y = y;

	m_pos.x = x - m_size.x / 2.0f;
	m_pos.y = y - m_size.y / 2.0f;

	m_rect.x = (int)(m_pos.x);
	m_rect.y = (int)(m_pos.y);

}

void Quad::setTopLeft(float x, float y)
{
	m_pos.x = x;
	m_pos.y = y;

	m_cen.x = x + m_size.x / 2.0f;
	m_cen.y = y + m_size.y / 2.0f;

	m_rect.x = (int)(m_pos.x);
	m_rect.y = (int)(m_pos.y);
}

void Quad::setSize(float w, float h)
{
	m_size.x = w;
	m_size.y = h;

	m_rect.w = (int)w;
	m_rect.h = (int)h;
}

