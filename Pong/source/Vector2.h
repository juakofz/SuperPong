#pragma once
#include <SDL.h>
#include <cmath>
#include <vector>

class Vector2
{
public:

	//Constructors
	Vector2(float px = 0, float py = 0);
	Vector2(SDL_Point p);

	//Coords
	float x, y;

	SDL_Point convert_int();

	//Operators
	Vector2 operator - (Vector2 v);
	Vector2 operator + (Vector2 v);
	Vector2 operator = (Vector2 v);
	Vector2 operator += (Vector2 v);
	Vector2 operator -= (Vector2 v);
	Vector2 operator = (SDL_Point p);

	Vector2 operator * (float k); //Multiply by scalar
	Vector2 operator / (float k); //Divide by scalar

	float operator * (Vector2 v); //Dot product

	void set(float px, float py);
	void zero();
	float dist(Vector2 dest); //Distance to vector
	Vector2 norm(float d = 1.0); //Get normalized vector (or resize to n)
	Vector2 rotate(float angle); //Rotate vector
	Vector2 normal(bool clockwise = true);
	float mod();
	float arg();

	static Vector2 angleToVec2(float a, bool deg = true);
	static std::vector<Vector2> getCorners(SDL_Rect rect);
	static Vector2 getCenter(SDL_Rect rect);
	static Vector2 getSegmentNormal(Vector2 p1, Vector2 p2);
	static bool intersectSegments(Vector2 o1, Vector2 f1, Vector2 o2, Vector2 f2, Vector2* intersec);
	static bool isPointInSegment(Vector2 o1, Vector2 f1, Vector2 p);
	static bool pointInRect(Vector2 p1, Vector2 cor1, Vector2 cor2);
};