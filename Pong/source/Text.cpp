#include "Text.h"

Text::Text()
{
	m_texture = NULL;
	m_font = NULL;
	m_size.x = 0;
	m_size.y = 0;

	p_renderer = NULL;
}

Text::Text(SDL_Renderer* renderer)
{
	m_texture = NULL;
	m_font = NULL;
	m_size.x = 0;
	m_size.y = 0;

	p_renderer = renderer;
}

Text::~Text()
{
	free();
}


bool Text::loadText(std::string text, std::string font_location, int font_size, SDL_Color color)
{
	//Loading success flag
	bool success = true;

	//Check font
	m_font = TTF_OpenFont(font_location.c_str(), font_size);
	if (m_font == NULL)
	{
		std::cout << "Failed to load font! SDL_ttf Error:" << std::endl
			<< TTF_GetError() << std::endl;

		success = false;
	}
	else
	{
		//Render Text
		if (!loadRenderedText(text.c_str(), color))
		{
			std::cout << "Failed to render Text: " << text << std::endl;
			success = false;
		}
	}

	return success;
}

bool Text::loadRenderedText(std::string text, SDL_Color color)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(m_font, text.c_str(), color);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		m_texture = SDL_CreateTextureFromSurface(p_renderer, textSurface);
		if (m_texture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			m_size.x = (int)textSurface->w;
			m_size.y = (int)textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return m_texture != NULL;
}

void Text::free()
{
	if (m_texture != NULL)
	{
		SDL_DestroyTexture(m_texture);
		m_texture = NULL;
		m_size.x = 0;
		m_size.y = 0;
	}
}

void Text::render(SDL_Renderer* renderer, int x, int y, int align_horizontal, int align_vertical, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	SDL_Rect renderQuad;

	//Espacio de renderizado
	int cx{ 0 }, cy{ 0 };

	switch (align_horizontal)
	{
	case LEFT:
		cx = x;
		break;
	case RIGHT:
		cx = x - (int)m_size.x;
		break;
	case CENTER:
		cx = x - (int)m_size.x/2;
		break;
	default:
		cx = x;
		break;
	}

	switch (align_vertical)
	{
	case TOP:
		cy = y;
		break;
	case BOTTOM:
		cy = y - (int)m_size.y;
		break;
	case CENTER:
		cy = y - (int)m_size.y / 2;
		break;
	default:
		cy = y;
		break;
	}

	renderQuad = { cx, cy, (int)m_size.x, (int)m_size.y };

	//Espacio final de renderizado de renderizado
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Renderizado
	SDL_RenderCopyEx(renderer, m_texture, clip, &renderQuad, angle, center, flip);
}
