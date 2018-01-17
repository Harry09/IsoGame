#pragma once

#include <Play\Objects\Entities\Entity.h>


class ParticleSystem;

class Enemy : public Entity
{
private:
	bool							m_damageAnim;
	sf::Color						m_lastSpriteColor;
	sf::Clock						m_lastAnimChange;
	const int						m_damageAnimDuration = 125;

	bool							m_deathAnim;
	int								m_animAlpha;

protected:
	bool							m_invincible;
	int								m_health;

	int								m_scoreValue;

	bool							m_moving;

protected:
	Enemy(const sf::Vector2f &pos);
	~Enemy();

	virtual void DeathEvent();

public:
	bool CollideWithObject(Object *object, bool blockMove = true) override;

	void Pulse(float deltaTime) override;
	void RenderGame(sf::RenderWindow *window) override;
	void Event(sf::Event *e) override;
};

