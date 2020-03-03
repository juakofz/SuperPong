#pragma once
#include <SDL.h>
#include "Globals.h"
#include "Vector2.h"


class GameObject
{
public:
	GameObject(): m_cen(0, 0), m_vel(0, 0) {};
	GameObject(float px, float py) : m_cen(px, py), m_vel(0, 0) {};
	~GameObject();

	virtual void render(SDL_Renderer* renderer) = 0;
	virtual void processKeys() = 0;

	//Getters and setters
	virtual void setCen(float cx, float cy) = 0;
	virtual void setCen(Vector2 cen) = 0;
	Vector2 getCen();

	void setVel(Vector2 vel);
	Vector2 getVel();

protected:
	
	//Position
	Vector2 m_cen;
	Vector2 m_next_cen;

	//Velocity
	Vector2 m_vel;

};

