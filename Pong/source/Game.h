#pragma once
#include "Paddle.h"
#include "Ball.h"
#include "InteractionObserver.h"
#include "DashedLine.h"
#include "Text.h"
#include "Timer.h"

//Charged of running the game itself
class Game
{
public:
	Game(SDL_Window* w, SDL_Renderer* r);
	void run();
	void render();


private:

	//Internal game state machine
	enum game_state_internal {
		e_sate_0,
		e_game_start,
		e_point_start,
		e_playing,
		e_point_scored,
		e_game_end,
	};
	int m_game_state{ e_sate_0 };

	//State: game start
	void runStateGameStart();
	void renderStateGameStart();
	int m_countdown_time{ 3 };
	int m_last_countdown{ m_countdown_time };
	Timer m_countdown_timer;
	Text m_countdown_text;

	//State: point start
	//...

	//State: playing
	void runStatePlaying();
	void renderStatePlaying();
	void renderStatePlayingTexture();
	SDL_Texture* p_playing_snapshot;

	//Internal elements
	SDL_Window* p_window; //Game window
	SDL_Renderer* p_renderer; //Game renderer
	InteractionObserver m_observer; //Interactions

	//Game elements
	Paddle m_pad_left, m_pad_right;
	Ball m_ball;
	SDL_Rect m_game_area; //Game area limits

	//Player score
	int m_score_p1{ 0 }, m_score_p2{ 0 };

	//Visual elements
	SDL_Rect m_white_border;
	DashedLine m_center_line; //Center dashed line

	//Text
	Text m_text_score_p1;
	Text m_text_score_p2;
	std::string m_font = "font/visitor1.ttf";
	//std::string font = "font/Gamer.ttf";
	int m_score_size = 80;
	int m_countdown_size = 150;

	SDL_Color m_color_white{ 0xFF, 0xFF, 0xFF, 0xFF }; //White
	SDL_Color m_color_black{ 0x00, 0x00, 0x00, 0xFF }; //White

};

