#pragma once

#include <SFML\System\Vector2.hpp>

bool IsFileExist(const char *filePath);

float pointDir(const sf::Vector2f &pos1, const sf::Vector2f &pos2);
sf::Vector2f lenDir(float len, float dir);

float lengthToPoint(const sf::Vector2f &pos1, const sf::Vector2f &pos2);

float toRadian(float deg);
float toDegree(float rad);

float degreeToSFMLRotate(float degree);
