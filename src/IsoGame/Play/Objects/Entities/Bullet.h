#pragma once

#include "Entity.h"

#include <memory>
#include <SFML\System\Vector2.hpp>

class Bullet : public Entity
{
public:
	enum Type
	{
		FRIENDLY,
		UNFRIENDLY
	};

private:
	Type			m_bulletType;

	float			m_direction;

	sf::Vector2f	m_startPos;

	float			m_damage;

	int				m_liveTicks;
	bool			m_inWall;

private:
	void Init();

public:
	Bullet(Type bulletType, const sf::Vector2f &startPos, const sf::Vector2f &velocity);
	Bullet(Type bulletType, const sf::Vector2f &startPos, float dir, float speed);
	~Bullet();

	auto GetBulletType() const { return m_bulletType; }

	void WallCollision(sf::FloatRect *walls) override;

	bool CollideWithObject(Object *object, bool blockMove = true) override;

	void SetDamage(float damage) { m_damage = damage; }
	float GetDamage() const { return m_damage; }

	void Pulse(float deltaTime) override;
	void RenderGame(sf::RenderWindow *window) override;
	void Event(sf::Event *e) override {}
};

