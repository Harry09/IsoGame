#include "Zombie.h"

#include <Play\Objects\Entities\Player.h>

Zombie::Zombie(const sf::Vector2f &pos)
	: Enemy(pos)
{
	LoadTextures("enemies\\zombie", Object::FULL_DIRECTIONS);
	SetTexture(Directions::S);
	m_health = 20;
	m_hitbox = sf::FloatRect(0, 30, 52, 29);


	m_scoreValue = 250;
}

void Zombie::Pulse(float deltaTime)
{
	auto thisPos = GetPosition();
	auto playerPos = Player::Get()->GetPosition();


	if (abs(thisPos.x - playerPos.x) < 5.f)
		m_horMoveDir = HOR_NONE;
	else
	{
		if (thisPos.x > playerPos.x)
			m_horMoveDir = LEFT;
		else if (thisPos.x < playerPos.x)
			m_horMoveDir = RIGHT;
	}

	if (abs(thisPos.y - playerPos.y) < 5.f)
		m_verMoveDir = VER_NONE;
	else
	{
		if (thisPos.y > playerPos.y)
			m_verMoveDir = UP;
		else if (thisPos.y < playerPos.y)
			m_verMoveDir = DOWN;
	}

	Enemy::Pulse(deltaTime);
}
