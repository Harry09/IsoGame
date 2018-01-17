#pragma once

#include "Enemy.h"

class Zombie : public Enemy
{
public:
	Zombie(const sf::Vector2f &pos);

	void Pulse(float deltaTime) override;
};

