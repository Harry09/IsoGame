#pragma once

#include "Enemy.h"

class Robot : public Enemy
{
private:
	sf::Clock		m_lastMove;

public:
	Robot(const sf::Vector2f &pos, bool invincible);
	~Robot();

	void Pulse(float deltaTime) override;
};

