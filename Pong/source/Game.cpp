#include "Game.h"

Game::Game(SDL_Window* w, SDL_Renderer* r, SDL_Texture* t, bool* fquit)
{
	//Start game state
	m_current_state = state_0;
	
	//Rendering
	p_window = w;
	p_renderer = r;
	p_render_texture = t;
	p_internal_texture = SDL_CreateTexture(p_renderer, SDL_PIXELFORMAT_RGBA32,
		SDL_TEXTUREACCESS_TARGET, g_screen_width, g_screen_height);

	//External quit flag
	f_quit = fquit;

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
	m_observer = InteractionObserver(&m_score_p1, &m_score_p2, &f_point_scored, &m_game_area);
	m_observer.add(&m_pad_left);
	m_observer.add(&m_pad_right);
	m_observer.add(&m_ball);

	//Center dashed line
	m_center_line = DashedLine(g_margin_x + g_game_area_x / 2, g_margin_top,
		g_px_size, g_game_area_y, 2 * g_px_size);

	//Text
	m_text_score_p1 = Text(p_renderer);
	m_text_score_p2 = Text(p_renderer);
	m_text_countdown = Text(p_renderer);
	m_text_end_winner = Text(p_renderer);
	m_text_end_info = Text(p_renderer);
	std::string font = "font/visitor1.ttf";
	//std::string font = "font/Gamer.ttf";
	int score_size = 80;

	SDL_Color color_white{ 0xFF, 0xFF, 0xFF, 0xFF }; //White
	SDL_Color color_black{ 0x00, 0x00, 0x00, 0xFF }; //White
}

Game::~Game()
{
	//Free texture
	SDL_DestroyTexture(p_internal_texture);

	//Free text
	m_text_score_p1.free();
	m_text_score_p2.free();
	m_text_pause.free();
	m_text_end_winner.free();
	m_text_end_info.free();
	m_text_countdown.free();
}

void Game::run()
{

	if (m_current_state == state_0)
	{
		setState(state_game_start);
	}
	else
	{
		runState(m_current_state);
	}

}

void Game::render()
{
	//Render to internal texture
	renderState(m_current_state);

	//Copy internal texture to main render texture
	SDL_SetRenderTarget(p_renderer, p_render_texture);
	SDL_RenderCopy(p_renderer, p_internal_texture, NULL, NULL);

}

void Game::togglePause()
{
	//Not in game end
	if (m_current_state == state_game_end)
	{
		return;
	}

	//pause game
	if (m_current_state != state_pause)
	{
		changeState(state_pause);
	}
	//unpause game
	else
		changeState(m_held_state);
}

void Game::setState(int next_state)
{
	//End current state
	endState(m_current_state);

	//Change state
	m_current_state = next_state;

	//Start next state
	initState(m_current_state);
}

void Game::changeState(int next_state)
{
	//End current state
	pauseState(m_current_state);

	//Change state
	m_held_state = m_current_state;
	m_current_state = next_state;

	//Start next state
	initState(m_current_state);
}

void Game::initState(int state)
{
	//Init state
	switch (state)
	{
	case state_game_start:
		initStateGameStart();
		break;

	case state_point_start:
		initStatePointStart();
		break;

	case state_playing:
		initStatePlaying();
		break;

	case state_pause:
		initStatePause();
		break;

	case state_game_end:
		initStateGameEnd();
		break;

	default:
		break;
	}
}

void Game::runState(int state)
{
	//Run state
	switch (state)
	{
	case state_game_start:
		runStateGameStart();
		break;

	case state_point_start:
		runStatePointStart();
		break;

	case state_playing:
		runStatePlaying();
		break;

	case state_pause:
		runStatePause();
		break;

	case state_game_end:
		runStateGameEnd();
		break;


	default:
		break;
	}
}

void Game::renderState(int state)
{
	//Render state
	switch (state)
	{
	case state_game_start:
		renderStateGameStart();
		break;

	case state_point_start:
		renderStatePointStart();
		break;

	case state_playing:
		renderStatePlaying();
		break;

	case state_pause:
		renderStatePause();
		break;

	case state_game_end:
		renderStateGameEnd();
		break;

	default:
		break;
	}
}

void Game::pauseState(int state)
{
	//Pause state
	switch (state)
	{
	case state_game_start:
		pauseStateGameStart();
		break;

	case state_point_start:
		pauseStatePointStart();
		break;

	case state_playing:
		pauseStatePlaying();
		break;

	case state_pause:
		pauseStatePause();
		break;

	case state_game_end:
		//not pausable
		break;

	default:
		break;
	}
}

void Game::endState(int state)
{
	//End state
	switch (state)
	{
	case state_game_start:
		endStateGameStart();
		break;

	case state_point_start:
		endStatePointStart();
		break;

	case state_playing:
		endStatePlaying();
		break;

	case state_pause:
		endStatePause();
		break;

	case state_game_end:
		endStateGameEnd();
		break;

	default:
		break;
	}
}

//--------------------------------------------------------------------- State: game start

void Game::initStateGameStart()
{
	//Reset paddle position
	m_pad_left.reset();
	m_pad_right.reset();

	//Reset score
	m_score_p1 = 0;
	m_score_p2 = 0;

	//Start countdown
	m_countdown_timer.start();
	m_last_countdown = m_countdown_time;
}

void Game::runStateGameStart()
{
	//Next state: point start
	if (m_countdown_timer.getSecs() >= m_countdown_time)
	{
		setState(state_point_start);
	}

	//Display start countdown
	if (m_countdown_timer.getSecs() >= m_countdown_time - m_last_countdown)
	{
		//std::cout << m_last_countdown-- << std::endl;

		m_text_countdown.free();
		m_text_countdown.loadText(std::to_string(m_last_countdown), m_font, m_countdown_size, m_color_white);
	
		m_last_countdown--;
	}
}

void Game::renderStateGameStart()
{
	//render game in the background
	renderStatePlaying();

	//Render to internal texture
	SDL_SetRenderTarget(p_renderer, p_internal_texture);

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

	m_text_countdown.render(p_renderer, g_screen_width / 2, g_screen_height / 2, CENTER, CENTER);

}

void Game::pauseStateGameStart()
{
	m_countdown_timer.pause();
}

void Game::endStateGameStart()
{
	m_countdown_timer.stop();
}

//--------------------------------------------------------------------- State: point start

void Game::initStatePointStart()
{
	//Update score and text
	m_text_score_p1.free();
	m_text_score_p1.loadText(std::to_string(m_score_p1), m_font, m_score_size, m_color_white);

	m_text_score_p2.free();
	m_text_score_p2.loadText(std::to_string(m_score_p2), m_font, m_score_size, m_color_white);

	m_blink_timer.start();
	f_render_ball = true;
	m_last_blink = m_blink_time;
}

void Game::runStatePointStart()
{
	//Key is down
	m_pad_left.processKeys();
	m_pad_right.processKeys();

	//Move only paddles
	m_observer.movePaddles();

	//Next State
	if (m_blink_timer.getTicks() >= m_blink_time)
	{
		setState(state_playing);
	}
}

void Game::renderStatePointStart()
{
	clearInternalTexture();

	//Render to internal texture
	SDL_SetRenderTarget(p_renderer, p_internal_texture);

	//Render background rectangles
	SDL_SetRenderDrawColor(p_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderFillRect(p_renderer, &m_white_border); //white line

	SDL_SetRenderDrawColor(p_renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(p_renderer, &m_game_area); //black interior

	//Render center lines
	m_center_line.render(p_renderer);

	//Render score text
	//SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	m_text_score_p1.render(p_renderer, m_score_pos_x_1, m_score_pos_y, RIGHT);
	m_text_score_p2.render(p_renderer, m_score_pos_x_2, m_score_pos_y, LEFT);

	//Render paddles and ball
	m_pad_left.render(p_renderer);
	m_pad_right.render(p_renderer);

	//Blinking ball
	if (m_blink_timer.getTicks() >= m_blink_time - m_last_blink)
	{
		f_render_ball = !f_render_ball;
		m_last_blink -= m_blink_interval;
	}

	if(f_render_ball)
	{
		m_ball.render(p_renderer);
	}
	//else
		//std::cout << "off" << std::endl;


}

void Game::pauseStatePointStart()
{
	m_blink_timer.pause();
}

void Game::endStatePointStart()
{
	m_blink_timer.stop();
}

//--------------------------------------------------------------------- State: playing

void Game::initStatePlaying()
{
	//nuthin (yet)
}

void Game::runStatePlaying()
{
	//Key is down
	m_pad_left.processKeys();
	m_pad_right.processKeys();

	//Move
	m_observer.movePaddles();
	m_observer.moveBalls();

	//Next state: point start
	if (f_point_scored)
	{
		//if max score, end game
		if (m_score_p1 >= m_max_score || m_score_p2 >= m_max_score)
		{
			f_point_scored = false;
			setState(state_game_end);
		}
		else
		{
			f_point_scored = false;
			setState(state_point_start);
		}

	}
}

void Game::renderStatePlaying()
{
	clearInternalTexture();

	//Render to internal texture
	SDL_SetRenderTarget(p_renderer, p_internal_texture);

	//Render background rectangles
	SDL_SetRenderDrawColor(p_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderFillRect(p_renderer, &m_white_border); //white line

	SDL_SetRenderDrawColor(p_renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(p_renderer, &m_game_area); //black interior

	//Render center lines
	m_center_line.render(p_renderer);

	//Update score text
	m_text_score_p1.free();
	m_text_score_p1.loadText(std::to_string(m_score_p1), m_font, m_score_size, m_color_white);

	m_text_score_p2.free();
	m_text_score_p2.loadText(std::to_string(m_score_p2), m_font, m_score_size, m_color_white);

	//Render score text
	//SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	m_text_score_p1.render(p_renderer, m_score_pos_x_1, m_score_pos_y, RIGHT);
	m_text_score_p2.render(p_renderer, m_score_pos_x_2, m_score_pos_y, LEFT);

	//Render paddles and ball
	m_pad_left.render(p_renderer);
	m_pad_right.render(p_renderer);
	m_ball.render(p_renderer);
}

void Game::pauseStatePlaying()
{
	//nuthin
}

void Game::endStatePlaying()
{
	//nuthin
}

//--------------------------------------------------------------------- State: pause

void Game::initStatePause()
{
	//nuthin
}

void Game::runStatePause()
{
	//TODO
}

void Game::renderStatePause()
{
	//Render background
	renderState(m_held_state);

	//Set render target to internal texture
	SDL_SetRenderTarget(p_renderer, p_internal_texture);

	//Add a dark transparent layer over the backround
	SDL_SetRenderDrawBlendMode(p_renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(p_renderer, 0, 0, 0, 128);
	SDL_Rect dark_layer{0,0,g_screen_width,g_screen_height};
	SDL_RenderFillRect(p_renderer, &dark_layer);

	//Render pause rectangle
	int cx = g_screen_width / 2;
	int cy = g_screen_height / 2;
	int w = 120;
	int h = 120;

	//Outer rectangle
	SDL_Rect rect_white{ cx - w / 2, cy - h / 2, w, h };
	SDL_SetRenderDrawColor(p_renderer, m_color_white.r, m_color_white.g, m_color_white.b, m_color_white.a);
	SDL_RenderFillRect(p_renderer, &rect_white);

	//Inner rectangle
	int thick = 15;
	w -= thick;
	h -= thick;
	SDL_Rect rect_black{ cx - w / 2, cy - h / 2, w, h };
	SDL_SetRenderDrawColor(p_renderer, m_color_black.r, m_color_black.g, m_color_black.b, m_color_black.a);
	SDL_RenderFillRect(p_renderer, &rect_black);

	//Render pause bars
	int bar_width = 20;
	int bar_height = 60;
	int bar_space = 20;

	//left bar
	SDL_Rect bar_left{ cx - (bar_width + bar_space / 2), cy - bar_height / 2, bar_width, bar_height};
	SDL_SetRenderDrawColor(p_renderer, m_color_white.r, m_color_white.g, m_color_white.b, m_color_white.a);
	SDL_RenderFillRect(p_renderer, &bar_left);

	//Right bar
	SDL_Rect bar_rigth{ cx + (bar_space / 2), cy - bar_height / 2, bar_width, bar_height };
	SDL_SetRenderDrawColor(p_renderer, m_color_white.r, m_color_white.g, m_color_white.b, m_color_white.a);
	SDL_RenderFillRect(p_renderer, &bar_rigth);


}

void Game::pauseStatePause()
{
	//nuthin
}

void Game::endStatePause()
{
	//nuthin
}

bool Game::checkPause()
{
	//Key is down
	const Uint8* state = SDL_GetKeyboardState(NULL);
	
	if (state[SDL_SCANCODE_P])
	{
		changeState(state_pause);
		return true;
	}

	return false;
}

//--------------------------------------------------------------------- State: game end

void Game::initStateGameEnd()
{
	//winner text
	int player = 0;
	if (m_score_p1 > m_score_p2)
	{
		player = 1;
	}
	else
	{
		player = 2;
	}

	std::string end_text = "Player " + std::to_string(player) + " wins!";
	m_text_end_winner.free();
	m_text_end_winner.loadText(end_text, m_font, m_end_text_size, m_color_white);

	//Info text
	std::string info_text = "Play again? Y/N";
	m_text_end_info.free();
	m_text_end_info.loadText(info_text, m_font, m_end_text_size, m_color_white);

}

void Game::runStateGameEnd()
{
	//Check keys
	const Uint8* state = SDL_GetKeyboardState(NULL);

	if (state[SDL_SCANCODE_Y]) {
		//play again
		setState(state_0);
	}
	if (state[SDL_SCANCODE_N]) {
		//Quit
		*f_quit = true;
	}
}

void Game::renderStateGameEnd()
{
	//Render background
	renderState(state_playing);

	//Set render target to internal texture
	SDL_SetRenderTarget(p_renderer, p_internal_texture);

	//Add a dark transparent layer over the backround
	SDL_SetRenderDrawBlendMode(p_renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(p_renderer, 0, 0, 0, 128);
	SDL_Rect dark_layer{ 0,0,g_screen_width,g_screen_height };
	SDL_RenderFillRect(p_renderer, &dark_layer);

	//Render pause rectangle
	int cx = g_screen_width / 2;
	int cy = g_screen_height / 2;
	int w = 500;
	int h = 120;

	//Outer rectangle
	SDL_Rect rect_white{ cx - w / 2, cy - h / 2, w, h };
	SDL_SetRenderDrawColor(p_renderer, m_color_white.r, m_color_white.g, m_color_white.b, m_color_white.a);
	SDL_RenderFillRect(p_renderer, &rect_white);

	//Inner rectangle
	int thick = 15;
	w -= thick;
	h -= thick;
	SDL_Rect rect_black{ cx - w / 2, cy - h / 2, w, h };
	SDL_SetRenderDrawColor(p_renderer, m_color_black.r, m_color_black.g, m_color_black.b, m_color_black.a);
	SDL_RenderFillRect(p_renderer, &rect_black);

	//Render text
	m_text_end_winner.render(p_renderer, cx, cy - m_end_text_size/2, CENTER, CENTER);
	m_text_end_info.render(p_renderer, cx, cy + m_end_text_size/2, CENTER, CENTER);
}

void Game::pauseStateGameEnd()
{
	//nuthin
}

void Game::endStateGameEnd()
{
	//nuthin
}

void Game::clearInternalTexture()
{
	//Render to internal texture
	SDL_SetRenderTarget(p_renderer, p_internal_texture);

	//Reset render color
	SDL_SetTextureColorMod(p_internal_texture, 255, 255, 255);

	//Reset alpha value
	SDL_SetTextureBlendMode(p_internal_texture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(p_internal_texture, 255);

	//Clear render texture to black
	setRenderDrawColorBlack(p_renderer);
	SDL_RenderClear(p_renderer);
}

