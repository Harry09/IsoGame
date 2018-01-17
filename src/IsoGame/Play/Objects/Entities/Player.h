#pragma once

#include <SFML\Graphics.hpp>

#include "Entity.h"

#include <vector>

class Bullet;
class Portal;
class Pickup;

class Player : public Entity
{
private:
	bool									m_resetDir;
	sf::Clock								m_resetDirTime;

	Directions::Direction					m_shotDir;
	Entity::HorDir							m_horShotDir;
	Entity::VerDir							m_verShotDir;

	bool									m_deathAnim;
	int										m_deathAnimStage;
	sf::Vector2f							m_ghostPos;
	float									m_sin;

	bool									m_shoting;
	sf::Clock								m_lastShotTime;
	const int								m_shotDelay = 750;

	float									m_bulletSpeed;

	bool									m_damageAnim;
	sf::Clock								m_lastAnimIndexChange;
	int										m_damageAnimIndex;
	const int								m_indexTime = 50;


	bool									m_invincible;

protected:
	// player stats
	int										m_health; // hearths
	float									m_bulletDamage;
	float									m_shootingSpeed;
	float									m_moveSpeed;
	friend Pickup;

public:
	static const int						MAX_HEALTH = 10;

private:
	const float								MIN_LEN_TO_PORTAL = 50.f;

	static Player *s_pInst;

public:
	Player(const sf::Vector2f &pos);
	~Player();

	bool CollideWithObject(Object *object, bool blockMove = true) override;

	void Pulse(float deltaTime) override;
	void RenderGame(sf::RenderWindow *window) override;
	void Event(sf::Event *e) override;

	void SetPos(sf::Vector2f &pos);

	bool IsNearPortal(Portal *portal);

	void TakeDamage();

	auto GetHealth()		const { return m_health;		}
	auto GetBulletDamage()	const { return m_bulletDamage;	}
	auto GetShootingSpeed() const { return m_shootingSpeed;	}
	auto GetMoveSpeed()		const { return m_moveSpeed;		}

	static Player *Get() { return s_pInst; }
};

