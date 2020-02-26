#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>
#include "Globals.h"
#include "Vector2.h"

class Text
{
public:
	Text(SDL_Renderer* renderer);
	~Text();

	bool loadText(std::string text, std::string font_location, int font_size, SDL_Color color);
	void free();
	void render(SDL_Renderer* renderer, int x, int y, int align = LEFT, SDL_Rect* clip = NULL,
		double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

private:

	bool loadRenderedText(std::string textureText, SDL_Color textColor);

	//SDL Texture object
	SDL_Texture* m_texture;
	//Size
	Vector2 m_size;
	//Text font
	TTF_Font* m_font;

	//Renderer
	SDL_Renderer* p_renderer;
};

