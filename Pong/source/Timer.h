#pragma once
#include <SDL.h>

class Timer
{
public:
	
	Timer();
	~Timer();

	//The various clock actions
	void start();
	void stop();
	void pause();
	void unpause();
	void restart();

	//Gets the timer's time
	Uint32 getTicks();

	//Gets the timer's time in seconds
	float getSecs();

	//Checks the status of the timer
	bool isStarted();
	bool isPaused();

private:
	//The clock time when the timer started
	Uint32 startTicks;

	//The ticks stored when the timer was paused
	Uint32 pausedTicks;

	//The timer status
	bool paused;
	bool started;
};

