#include "Boss1.h"

#include <Common\Random.h>

#include <Play\Objects\Entities\Enemies\Robot.h>
#include <Play\Rooms\Room.h>

#include <SFML\Graphics.hpp>

Boss1::Boss1(const sf::Vector2f &pos)
	: Boss(pos)
{
	LoadTextures("enemies\\spaceCraft1", Object::FULL_DIRECTIONS);
	SetTexture(Directions::S);

	m_hitbox = sf::FloatRect(64.f, 22.f, 85.f, 91.f);

	m_health = m_maxHealth = 200;

	m_scoreValue = 5000;
}

Boss1::~Boss1()
{

}

void Boss1::Pulse(float deltaTime)
{
	if (m_lastRobotCreated.getElapsedTime().asMilliseconds() > 10000)
	{
		m_pParent->AddObject(new Robot(GetPosition(), false));
		m_lastRobotCreated.restart();
	}

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

	Boss::Pulse(deltaTime);
}
