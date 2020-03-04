#include "DashedLine.h"

DashedLine::DashedLine(int x, int y, int w, int h, int dash_size, bool vertical)
{
	m_ox = x;
	m_oy = y;
	m_width = w;
	m_height = h;
	m_dash_size = dash_size;

	int length;
	if (vertical) length = h;
	else length = w;

	for (int i = 0; i < (length/dash_size); i++)
	{
		v_rects.emplace_back(SDL_Rect{ m_ox, m_oy + i * m_dash_size, m_width, m_dash_size });
	}

	if (length % dash_size)
	{
		v_rects.emplace_back(SDL_Rect{ m_ox, m_oy + (length / dash_size) * m_dash_size, m_width, (length % dash_size) });
	}

}

DashedLine::DashedLine()
{
	m_ox = 0;
	m_oy = 0;
	m_width = 0;
	m_height = 0;
	m_dash_size = 0;

	
}

DashedLine& DashedLine::operator=(const DashedLine& obj)
{
	m_ox = obj.m_ox;
	m_oy = obj.m_oy;
	m_width = obj.m_width;
	m_height = obj.m_height;
	m_dash_size = obj.m_dash_size;

	//Rectangles
	v_rects = obj.v_rects;

	return *this;
}

DashedLine::~DashedLine()
{
}

void DashedLine::render(SDL_Renderer* renderer)
{
	int i = 0;
	for (auto it = v_rects.begin(); it != v_rects.end(); it++)
	{
		i++;
		if (i % 2)
		{
			SDL_SetRenderDrawColor(renderer, m_color_1.r, m_color_1.g, m_color_1.b, m_color_1.a);
		}
		else
		{
			SDL_SetRenderDrawColor(renderer, m_color_2.r, m_color_2.g, m_color_2.b, m_color_2.a);
		}
		SDL_Rect r = *it;
		SDL_RenderFillRect(renderer, &r);
	}
}
