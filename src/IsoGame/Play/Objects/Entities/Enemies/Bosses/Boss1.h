#pragma once

#include "Boss.h"



class Boss1 : public Boss
{
private:
	sf::Clock								m_lastMove;

	sf::Clock								m_lastRobotCreated;

public:
	Boss1(const sf::Vector2f &pos);
	~Boss1();

	void Pulse(float deltaTime) override;
};

