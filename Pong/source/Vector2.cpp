#include "Vector2.h"
#include <algorithm>
#include <math.h>

//Constructor
Vector2::Vector2(float px, float py)
{
	x = px;
	y = py;
}

Vector2::Vector2(SDL_Point p)
{
	x = static_cast<float>(p.x);
	y = static_cast<float>(p.y);
}

//Destructor
Vector2::~Vector2()
{
}

SDL_Point Vector2::convert_int()
{
	SDL_Point ret;
	ret.x = x;
	ret.y = y;

	return ret;
}

float Vector2::mod()
{
	return sqrt(x * x + y * y);
}

float Vector2::arg()
{
	float aux;
	aux = 180 * atan2(y, x) / M_PI;
	if (aux < 0) return (aux + 360);
	else return aux;
	//return (180 * atan2(y, x) / M_PI);
}

Vector2 Vector2::operator + (Vector2 v)
{
	return Vector2(x + v.x, y + v.y);
}

Vector2 Vector2::operator - (Vector2 v)
{
	return Vector2(x - v.x, y - v.y);
}

Vector2 Vector2::operator = (Vector2 v)
{
	x = v.x;
	y = v.y;
	return *this;
}

Vector2 Vector2::operator += (Vector2 v)
{
	x += v.x;
	y += v.y;
	return *this;
}

Vector2 Vector2::operator -= (Vector2 v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

Vector2 Vector2::operator = (SDL_Point p)
{
	x = static_cast<float>(p.x);
	y = static_cast<float>(p.y);
	return(Vector2(x, y));
}

Vector2 Vector2::operator * (float k)
{
	return Vector2(k * x, k * y);
}

void Vector2::set(float px, float py)
{
	x = px;
	y = py;
}

void Vector2::zero()
{
	x = 0;
	y = 0;
}

float Vector2::dist(Vector2 dest)
{
	//Vector2 aux = *this - dest;
	Vector2 aux = dest - *this;
	return aux.mod();
}

Vector2 Vector2::norm(float d)
{
	Vector2 aux;
	aux.x = d * x / mod();
	aux.y = d * y / mod();
	return aux;
}

Vector2 Vector2::normal(bool right)
{
	float angle = arg();
	if (right) angle += 90;
	else angle -= 90;
	Vector2 aux = angleToVec2(angle);
	aux = aux * mod();
	return aux;
}

Vector2 Vector2::angleToVec2(float a)
{
	Vector2 aux;
	aux.x = cos(a);
	aux.y = sin(a);
	return aux;
}

std::vector<Vector2> Vector2::getCorners(SDL_Rect rect)
{
	std::vector<Vector2> aux;
	aux.emplace_back(Vector2{ (float)rect.x, (float)rect.y });
	aux.emplace_back(Vector2{ (float)rect.x + (float)rect.w, (float)rect.y });
	aux.emplace_back(Vector2{ (float)rect.x, (float)rect.y + (float)rect.h });
	aux.emplace_back(Vector2{ (float)rect.x + (float)rect.w, (float)rect.y + (float)rect.h });

	return aux;
}

Vector2 Vector2::getCenter(SDL_Rect rect)
{
	return Vector2{ (float)rect.x + (float)rect.w / 2, (float)rect.y + (float)rect.h / 2 };
}


bool Vector2::intersectSegments(Vector2 o1, Vector2 f1, Vector2 o2, Vector2 f2, Vector2* intersec)
{
	//Calculates the intersection between two segment lines. Returns false if no intersection

	//Calculate line parameters (vector parameter form)

	//Line segment 1: from o1 to f1
	Vector2 v1 = f1 - o1;

	//Line segment 2: from o2 to f2
	Vector2 v2 = f2 - o2;

	//Intersection point (solving equation)

	//Calculate intersection
	Vector2 intersection;
	intersection.x = (o1.x * v2.x - o2.x * v1.x) / (v2.x - v1.x);
	intersection.y = (o1.y * v2.y - o2.y * v1.y) / (v2.y - v1.y);

	/*float min_x = std::min({ o1.x, o2.x, f1.x, f2.x });
	float min_y = std::min({ o1.y, o2.y, f1.y, f2.y });
	float max_x = std::max({ o1.x, o2.x, f1.x, f2.x });
	float max_y = std::max({ o1.y, o2.y, f1.y, f2.y });
	SDL_Rect bound{ min_x, min_y, max_x - min_x, max_y - min_y };*/

	//Check for intersection inside region
	SDL_Rect bound1{ std::min(o1.x, f1.x), std::min(o1.y, f1.y), abs(f1.x - o1.x), abs(f1.y - o1.y)};
	SDL_Rect bound2{ std::min(o2.x, f2.x), std::min(o2.y, f2.y), abs(f2.x - o2.x), abs(f2.y - o2.y) };

	if (pointInRect(intersection, o1, f1) && pointInRect(intersection, o2, f2))
	{
		*intersec = intersection;
		return true;
	}
	else {
		intersec = NULL;
		return false;
	}

}

bool Vector2::pointInRect(Vector2 p1, Vector2 cor1, Vector2 cor2)
{
	if ((p1.x >= std::min(cor1.x, cor2.x)) && (p1.y >= std::min(cor1.y, cor2.y)) &&
		(p1.x <= std::max(cor1.x, cor2.x)) && (p1.y <= std::max(cor1.y, cor2.y)))
		return true;
	else
		return false;
}