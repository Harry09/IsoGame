#include "Shooter.h"

#include <Common\Common.h>
#include <Common\Random.h>

#include <Play\Objects\Entities\Bullet.h>
#include <Play\Objects\Entities\Player.h>
#include <Play\Rooms\Room.h>

Shooter::Shooter(const sf::Vector2f &pos)
	: Enemy(pos)
{
	LoadTextures("enemies\\shooter", Object::FULL_DIRECTIONS);
	SetTexture(Directions::S);
	m_pSprite->setColor(sf::Color(0x8991ffFF));

	m_health = 15;

	m_bulletSpeed = 5;

	m_moving = false;
	m_hitbox = sf::FloatRect(0, 30, 52, 29);

	m_scoreValue = 500;
}

Shooter::~Shooter()
{
}

void Shooter::Pulse(float deltaTime)
{
	if (m_lastMove.getElapsedTime().asMilliseconds() > m_moveDuration)
	{
		m_horMoveDir = HOR_NONE;
		m_verMoveDir = VER_NONE;

		m_moving = false;
	}

	Random random;

	if (!m_moving)
	{
		int randomMove = random.Get<int>(1, 3);

		if (randomMove == 1)
		{
			int moveDir = random.Get<int>(1, 3);

			if (moveDir == 1)
				m_horMoveDir = LEFT;
			else if (moveDir == 2)
				m_horMoveDir = RIGHT;
			else
				m_horMoveDir = HOR_NONE;

			m_moveDuration = random.Get<int>(1000, 3000);

			m_lastMove.restart();

			m_moving = true;
		}

		randomMove = random.Get<int>(1, 3);

		if (randomMove == 1)
		{
			int moveDir = random.Get<int>(1, 2);

			if (moveDir == 1)
				m_verMoveDir = UP;
			else if (moveDir == 2)
				m_verMoveDir = DOWN;
			else
				m_verMoveDir = VER_NONE;

			m_moveDuration = random.Get<int>(1000, 3000);

			m_lastMove.restart();

			m_moving = true;
		}
	}

	if (m_lastShot.getElapsedTime().asMilliseconds() > 2000)
	{
		auto player = Player::Get();

		float lenToPlayer = lengthToPoint(GetPosition(), player->GetPosition());

		float a = lenToPlayer / m_bulletSpeed / 2;

		auto newPlayerPos = player->GetPosition() + player->GetVelocity() * a;

		float dir = pointDir(GetPosition(), newPlayerPos);

		m_pParent->AddObject(new Bullet(Bullet::UNFRIENDLY, GetPosition(), lenDir(m_bulletSpeed, dir)));

		m_lastShot.restart();
	}

	Enemy::Pulse(deltaTime);
}
