#pragma once
#include <SDL.h>

class GameObject
{
public:
	GameObject(): m_c_x(0), m_c_y(0), m_v_x(0), m_v_y(0) {};
	GameObject(float px, float py) : m_c_x(px), m_c_y(px), m_v_x(0), m_v_y(0) {};
	~GameObject();

	virtual void render(SDL_Renderer* renderer) = 0;
	virtual void processKeys() = 0;

protected:
	
	//Position
	float m_c_x, m_c_y;
	//Velocity
	float m_v_x, m_v_y;

};

