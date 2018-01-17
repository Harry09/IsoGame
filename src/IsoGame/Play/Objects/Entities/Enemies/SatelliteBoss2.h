#pragma once
#include "Enemy.h"
class SatelliteBoss2 : public Enemy
{
private:
	sf::Clock		m_lastIndexChange;
	int				m_texIndex;

public:
	SatelliteBoss2(const sf::Vector2f &pos);
	~SatelliteBoss2();

	void Pulse(float deltaTime) override;
};

