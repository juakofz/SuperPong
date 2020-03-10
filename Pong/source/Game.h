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
	Game(SDL_Window* w, SDL_Renderer* r, SDL_Texture* t, bool* fquit);
	~Game();
	void run();
	void render();
	void togglePause();


private:

	//Internal game state machine
	enum game_state_internal {
		state_0,
		state_game_start,
		state_point_start,
		state_playing,
		state_point_scored,
		state_pause,
		state_game_end,
	};
	int m_current_state{ state_0 };
	int m_held_state{ state_0 };

	//State change
	void setState(int next_state); //Ends current state and changes to next state
	void changeState(int next_state); //Pauses current state and changes to next state

	//State functions
	void initState(int state);
	void runState(int state);
	void renderState(int state);
	void pauseState(int state);
	void endState(int state);

	//-------------- State: game start ---------------
	void initStateGameStart();
	void runStateGameStart();
	void renderStateGameStart();
	void pauseStateGameStart();
	void endStateGameStart();
	//State variables and objects
	int m_countdown_time{ 3 };
	int m_last_countdown{ m_countdown_time };
	Timer m_countdown_timer;
	Text m_text_countdown;
	int m_countdown_size = 150;

	//-------------- State: point start --------------
	void initStatePointStart();
	void runStatePointStart();
	void renderStatePointStart();
	void pauseStatePointStart();
	void endStatePointStart();
	//State variables and objects
	int m_blink_time{ 2000 };
	int m_blink_interval{ 500 };
	int m_last_blink{ m_blink_time };
	bool f_render_ball{ true };
	Timer m_blink_timer;

	//-------------- State: playing ------------------
	void initStatePlaying();
	void runStatePlaying();
	void renderStatePlaying();
	void pauseStatePlaying();
	void endStatePlaying();
	//State variables and objects
	bool f_point_scored{ false };

	//-------------- State: pause --------------------
	void initStatePause();
	void runStatePause();
	void renderStatePause();
	void pauseStatePause();
	void endStatePause();
	//State variables and objects
	Text m_text_pause;
	bool checkPause();

	//-------------- State: game end -----------------
	void initStateGameEnd();
	void runStateGameEnd();
	void renderStateGameEnd();
	void pauseStateGameEnd();
	void endStateGameEnd();
	//State variables and objects
	Text m_text_end_winner;
	Text m_text_end_info;
	int m_end_text_size = 50;

	//------------------------------------------------

	//Clear internal texture
	void clearInternalTexture();

	//------------- Variables and objects ------------

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
	int m_max_score = 11;
	int m_score_pos_x_1 = g_screen_width / 2 - 10;
	int m_score_pos_x_2 = g_screen_width / 2 + 17;
	int m_score_pos_y = g_margin_top;

	//Visual elements
	SDL_Rect m_white_border;
	DashedLine m_center_line; //Center dashed line
	Text m_text_score_p1;
	Text m_text_score_p2;
	std::string m_font = "font/visitor1.ttf";
	//std::string font = "font/Gamer.ttf";
	int m_score_size = 80;

	SDL_Color m_color_white{ 0xFF, 0xFF, 0xFF, 0xFF }; //White
	SDL_Color m_color_black{ 0x00, 0x00, 0x00, 0xFF }; //White

	//Texture to render
	SDL_Texture* p_render_texture;
	SDL_Texture* p_internal_texture;

	//External quit flag
	bool* f_quit;

};

