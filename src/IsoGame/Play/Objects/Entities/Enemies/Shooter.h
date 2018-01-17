#pragma once

#include "Enemy.h"

class Shooter :	public Enemy
{
private:
	bool			m_moving;
	sf::Clock		m_lastMove;

	sf::Clock		m_lastShot;

	int				m_moveDuration;

	float			m_bulletSpeed;

public:
	Shooter(const sf::Vector2f &pos);
	~Shooter();

	void Pulse(float deltaTime) override;
};

