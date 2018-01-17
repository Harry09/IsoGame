#pragma once
#include "Object.h"
class Wall : public Object
{

public:
	static char *TEXTURE_NAME;

	static float SPRITE_SCALE;

public:
	Wall(const sf::Vector2f &pos, bool scaleWall = true);
	~Wall();

	virtual void Pulse(float deltaTime) override { }
	virtual void RenderGame(sf::RenderWindow * window) override;
	virtual void Event(sf::Event * e) override {}
};

