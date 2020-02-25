#include "GameObject.h"

GameObject::~GameObject()
{
	return;
}

Vector2 GameObject::getPos()
{
	return m_cen;
}

void GameObject::setVel(Vector2 vel)
{
	m_vel = vel;
}

Vector2 GameObject::getVel()
{
	return m_vel;
}
