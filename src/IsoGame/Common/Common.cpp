#define _USE_MATH_DEFINES

#include "Common.h"

#include <fstream>
#include <cmath>

bool IsFileExist(const char *filePath)
{
	std::ifstream f(filePath);
	return f.good();
}

float pointDir(const sf::Vector2f &pos1, const sf::Vector2f &pos2)
{
	float deg = toDegree(-atan2(pos2.y - pos1.y, pos2.x - pos1.x));

	if (deg > 0.f)
		return deg;
	else if (deg < 0.f)
		return 360.f + deg;
	else
		return deg;
}

sf::Vector2f lenDir(float len, float dir)
{
	float rad = toRadian(dir);

	sf::Vector2f vec;
	vec.x = static_cast<float>(cos(rad)) * len;
	vec.y = -static_cast<float>(sin(rad)) * len;
	return vec;
}

float lengthToPoint(const sf::Vector2f &pos1, const sf::Vector2f &pos2)
{
	float cx = 0.f, cy = 0.f;

	cx = pos2.x - pos1.x;
	cy = pos2.y - pos1.y;

	return sqrt(pow(cx, 2) + pow(cy, 2));
}


float toRadian(float deg)
{
	return static_cast<float>(deg * M_PI / 180);
}

float toDegree(float rad)
{
	return static_cast<float>(rad * 180 / M_PI);
}

float degreeToSFMLRotate(float degree)
{
	if (degree < 0.f)
		return (-degree);

	return (360.f - degree);
}