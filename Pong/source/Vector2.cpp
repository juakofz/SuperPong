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

Vector2 Vector2::operator / (float k)
{
	return Vector2(k / x, k / y);
}

float Vector2::operator * (Vector2 v)
{
	return (x * v.x +  y * v.y);
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

Vector2 Vector2::normal(bool clockwise)
{
	Vector2 aux;

	float angle = arg();
	if (clockwise)
		aux.set(y, -x);
	else
		aux.set(-y, x);
	return aux.norm();
}

Vector2 Vector2::angleToVec2(float a, bool deg)
{
	Vector2 aux;
	if(deg) a = a * M_PI / 180; //Convert to rad
	aux.x = cos(a);
	aux.y = sin(a);
	return aux;
}

std::vector<Vector2> Vector2::getCorners(SDL_Rect rect)
{
	//Converts a rect into its 4 corners, from top left clockwise
	std::vector<Vector2> aux;
	aux.emplace_back(Vector2{ (float)rect.x, (float)rect.y }); //Top left
	aux.emplace_back(Vector2{ (float)rect.x + (float)rect.w, (float)rect.y }); //Top right
	aux.emplace_back(Vector2{ (float)rect.x + (float)rect.w, (float)rect.y + (float)rect.h }); //Bottom right
	aux.emplace_back(Vector2{ (float)rect.x, (float)rect.y + (float)rect.h }); // Bottom left

	return aux;
}

Vector2 Vector2::getCenter(SDL_Rect rect)
{
	return Vector2{ (float)rect.x + (float)rect.w / 2, (float)rect.y + (float)rect.h / 2 };
}

Vector2 Vector2::getSegmentNormal(Vector2 p1, Vector2 p2)
{
	Vector2 line(p2 - p1);
	return line.normal(true);
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
	//intersection.x = (o1.x * v2.x - o2.x * v1.x) / (v2.x - v1.x);
	//intersection.y = (o1.y * v2.y - o2.y * v1.y) / (v2.y - v1.y);

	float alpha = ( v1.x * (o2.y - o1.y) - v1.y * (o2.x - o1.x) ) / ( v1.y * v2.x - v1.x * v2.y );
	intersection = o2 + v2 * alpha;

	/*float min_x = std::min({ o1.x, o2.x, f1.x, f2.x });
	float min_y = std::min({ o1.y, o2.y, f1.y, f2.y });
	float max_x = std::max({ o1.x, o2.x, f1.x, f2.x });
	float max_y = std::max({ o1.y, o2.y, f1.y, f2.y });
	SDL_Rect bound{ min_x, min_y, max_x - min_x, max_y - min_y };*/

	//Check for intersection inside region
	//SDL_Rect bound1{ (int)std::min(o1.x, f1.x), (int)std::min(o1.y, f1.y), (int)abs(f1.x - o1.x), (int)abs(f1.y - o1.y)};
	//SDL_Rect bound2{ (int)std::min(o2.x, f2.x), (int)std::min(o2.y, f2.y), (int)abs(f2.x - o2.x), (int)abs(f2.y - o2.y) };

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

bool Vector2::isPointInSegment(Vector2 o1, Vector2 f1, Vector2 p)
{
	//Check using distance sum
	float d_total = o1.dist(f1);
	float d1 = o1.dist(p);
	float d2 = f1.dist(p);

	//Tolerance to accomodate floating point error
	float tol = 0.01;

	return ( (d1 + d2 >= d_total - tol) && (d1 + d2 <= d_total + tol) );
}

bool Vector2::pointInRect(Vector2 p1, Vector2 cor1, Vector2 cor2)
{
	//Tolerance to accomodate floating point error when dimensions are too close (liek for horizontal or vertical lines)
	float tol = 0.01;
	float tol_x = 0;
	float tol_y = 0;
	if (abs(cor1.x - cor2.x) < tol)
		tol_x = tol;
	if (abs(cor1.y - cor2.y) < tol)
		tol_y = tol;

	if ((p1.x >= std::min(cor1.x, cor2.x) - tol_x) && (p1.y >= std::min(cor1.y, cor2.y) - tol_y) &&
		(p1.x <= std::max(cor1.x, cor2.x) + tol_x) && (p1.y <= std::max(cor1.y, cor2.y) + tol_y))
		return true;
	else
		return false;
}