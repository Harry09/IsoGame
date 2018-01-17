#pragma once

#include "Boss.h"

class FinalBoss : public Boss
{
private:
	sf::Clock		m_lastShot;
	float			m_dir;
	int				m_stage;

private:
	void Shot(float dir, bool changeTex = false);

protected:
	void DeathEvent() override;

public:
	FinalBoss(const sf::Vector2f &pos);
	~FinalBoss();

	void Pulse(float deltaTime) override;
};

