#include "Robot.h"

#include <Common\Random.h>

Robot::Robot(const sf::Vector2f &pos, bool invincible)
	: Enemy(pos)
{
	m_acceleration = 1.f;
	m_maxSpeed = 3.f;

	if (!invincible)
	{
		m_health = 5;
	}
	else
		m_invincible = true;

	LoadTextures("enemies\\robot", Object::HALF_DIRECTIONS);
	SetTexture(Directions::NE);

	m_hitbox = sf::FloatRect(m_pSprite->getTextureRect());

	m_scoreValue = 100;

	m_particlesColor = sf::Color(0xf77c5bFF);
}

Robot::~Robot()
{
}

void Robot::Pulse(float deltaTime)
{
	if (m_lastMove.getElapsedTime().asMilliseconds() > 2000)
	{
		Random random;

		int moveDir = random.Get<int>(1, 2);

		if (moveDir == 1)
			m_horMoveDir = LEFT;
		else if (moveDir == 2)
			m_horMoveDir = RIGHT;


		moveDir = random.Get<int>(1, 2);

		if (moveDir == 1)
			m_verMoveDir = UP;
		else if (moveDir == 2)
			m_verMoveDir = DOWN;

		m_lastMove.restart();
	}

	if (m_horCollMoveBlock == LEFT)
		m_horMoveDir = RIGHT;
	else if (m_horCollMoveBlock == RIGHT)
		m_horMoveDir = LEFT;

	if (m_verCollMoveBlock == UP)
		m_verMoveDir = DOWN;
	else if (m_verCollMoveBlock == DOWN)
		m_verMoveDir = UP;

	Enemy::Pulse(deltaTime);
}
