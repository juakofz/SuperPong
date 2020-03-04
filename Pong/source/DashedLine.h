#pragma once
#include <SDL.h>
#include <vector>
	
class DashedLine
{
public:

	DashedLine(int x, int y, int w, int h, int dash_size, bool vertical = true);
	DashedLine();
	DashedLine& operator=(const DashedLine& obj);

	~DashedLine();
	void render(SDL_Renderer* renderer);

private:
	
	int m_ox;
	int m_oy;
	int m_width;
	int m_height;

	int m_dash_size;

	//Colors
	const SDL_Color m_color_1{ 0xFF, 0xFF, 0xFF, 0xFF }; //White
	const SDL_Color m_color_2{ 0x00, 0x00, 0x00, 0xFF }; //Black

	//Rectangles
	std::vector<SDL_Rect> v_rects;

};

