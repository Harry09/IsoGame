#pragma once

#include "Object.h"

#include <SFML\System\Clock.hpp>

class Player;

class Pickup : public Object
{
public:
	enum Type
	{
		NONE = 0,
		HEALTH,
		DAMAGE,
		MOVE_SPEED,
		SHOOTING_SPEED
	};
	
private:
	Type				m_type;

	float				m_floating;

	sf::Clock			m_lastIndexChange;
	int					m_texIndex;

	sf::Vector2f		m_startPos;

public:
	// type == 0 == random
	Pickup(const sf::Vector2f &pos, Pickup::Type type = NONE);
	~Pickup() = default;

	void Active(Player *player);

	virtual void Pulse(float deltaTime) override;
	virtual void RenderGame(sf::RenderWindow * window) override;
	virtual void Event(sf::Event * e) override;
};

