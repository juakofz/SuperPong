#include "Game.h"

Game::Game(SDL_Window* w, SDL_Renderer* r)
{
	//Start game state
	m_game_state = e_sate_0;
	
	//Rendering
	p_window = w;
	p_renderer = r;
	p_playing_snapshot = SDL_CreateTexture(p_renderer, SDL_PIXELFORMAT_RGBA32,
		SDL_TEXTUREACCESS_TARGET, g_screen_width, g_screen_height);

	//Paddles
	m_pad_left = Paddle(PLAYER_1, (float)g_paddle_margin_x, (float)(g_margin_top + g_game_area_y / 2));
	m_pad_right = Paddle(PLAYER_2, (float)(g_screen_width - g_paddle_margin_x), (float)(g_margin_top + g_game_area_y / 2));

	//Ball
	m_ball = Ball((float)g_screen_width / 2.0, (float)g_screen_height / 2.0, Vector2{ 1, -1 }, &m_score_p1, &m_score_p2);

	//Background rectangle. Black rect follows interior perimeter (game area)
	m_white_border = SDL_Rect{ g_margin_x - g_thickness, g_margin_top - g_thickness,
					   g_game_area_x + 2 * g_thickness , g_game_area_y + 2 * g_thickness };
	m_game_area = SDL_Rect{ g_margin_x, g_margin_top,
					   g_game_area_x, g_game_area_y };


	//Interaction observer
	m_observer = InteractionObserver(&m_score_p1, &m_score_p2, &m_game_area);
	m_observer.add(&m_pad_left);
	m_observer.add(&m_pad_right);
	m_observer.add(&m_ball);

	//Center dashed line
	m_center_line = DashedLine(g_margin_x + g_game_area_x / 2, g_margin_top,
		g_px_size, g_game_area_y, 2 * g_px_size);

	//Text
	m_text_score_p1 = Text(p_renderer);
	m_text_score_p2 = Text(p_renderer);
	m_countdown_text = Text(p_renderer);
	std::string font = "font/visitor1.ttf";
	//std::string font = "font/Gamer.ttf";
	int score_size = 80;

	SDL_Color color_white{ 0xFF, 0xFF, 0xFF, 0xFF }; //White
	SDL_Color color_black{ 0x00, 0x00, 0x00, 0xFF }; //White
}

void Game::run()
{
	switch (m_game_state)
	{
	case e_sate_0:
		m_countdown_timer.start();
		m_game_state = e_game_start;
		//break; //Do the game start thing

	case e_game_start:
		runStateGameStart();
		break;

	case e_playing:
		runStatePlaying();
		break;

	default:
		runStatePlaying();
		break;
	}
}

void Game::render()
{
	switch (m_game_state)
	{
	case e_game_start:
		renderStateGameStart();
		break;

	case e_playing:
		renderStatePlaying();
		break;

	default:
		renderStatePlaying();
		break;
	}
}

void Game::runStateGameStart()
{
	//Start playing
	if (m_countdown_timer.getSecs() >= m_countdown_time)
	{
		m_game_state = e_playing;
		//return;
	}

	//Display countdown
	if (m_countdown_timer.getSecs() >= m_countdown_time - m_last_countdown)
	{
		//std::cout << m_last_countdown-- << std::endl;

		m_countdown_text.free();
		m_countdown_text.loadText(std::to_string(m_last_countdown), m_font, m_countdown_size, m_color_white);
	
		m_last_countdown--;
	}
}

void Game::renderStateGameStart()
{
	//render game in the background
	renderStatePlayingTexture();
	SDL_SetRenderTarget(p_renderer, p_playing_snapshot);

	//render countdown graphics
	int cx = g_screen_width / 2;
	int cy = g_screen_height / 2;
	int w = 150;
	int h = 150;

	SDL_Rect rect_white{ cx - w/2, cy - h/2, w, h };
	SDL_SetRenderDrawColor(p_renderer, m_color_white.r, m_color_white.g, m_color_white.b, m_color_white.a);
	SDL_RenderFillRect(p_renderer, &rect_white);

	int thick = 15;
	w -= thick;
	h -= thick;

	SDL_Rect rect_black{ cx - w / 2, cy - h / 2, w, h };
	SDL_SetRenderDrawColor(p_renderer, m_color_black.r, m_color_black.g, m_color_black.b, m_color_black.a);
	SDL_RenderFillRect(p_renderer, &rect_black);

	m_countdown_text.render(p_renderer, g_screen_width / 2, g_screen_height / 2, CENTER, CENTER);

	SDL_SetRenderTarget(p_renderer, NULL);
	SDL_RenderCopy(p_renderer, p_playing_snapshot, NULL, NULL);
}

void Game::runStatePlaying()
{
	//Key is down
	m_pad_left.processKeys();
	m_pad_right.processKeys();

	//Move
	m_observer.movePaddles();
	m_observer.moveBalls();

	//Update score and text
	m_text_score_p1.free();
	m_text_score_p1.loadText(std::to_string(m_score_p1), m_font, m_score_size, m_color_white);

	m_text_score_p2.free();
	m_text_score_p2.loadText(std::to_string(m_score_p2), m_font, m_score_size, m_color_white);
}

void Game::renderStatePlaying()
{

	//Render background rectangles
	SDL_SetRenderDrawColor(p_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderFillRect(p_renderer, &m_white_border); //white line

	SDL_SetRenderDrawColor(p_renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(p_renderer, &m_game_area); //black interior

	//Render center lines
	m_center_line.render(p_renderer);

	//Render score text
	//SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	m_text_score_p1.render(p_renderer, g_screen_width / 2 - 10, g_margin_top, RIGHT);
	m_text_score_p2.render(p_renderer, g_screen_width / 2 + 22, g_margin_top, LEFT);

	//Render paddles and ball
	m_pad_left.render(p_renderer);
	m_pad_right.render(p_renderer);
	m_ball.render(p_renderer);
}

void Game::renderStatePlayingTexture()
{
	//SDL_RenderClear(p_renderer);
	SDL_SetTextureBlendMode(p_playing_snapshot, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(p_renderer, p_playing_snapshot);
	renderStatePlaying();
}

