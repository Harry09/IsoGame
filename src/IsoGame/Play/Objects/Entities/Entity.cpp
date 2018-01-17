#include "Entity.h"

Entity::Entity(const sf::Vector2f & pos)
	: Object(pos)
{
	m_entity = true;
	ResetMoveBlocks();
}

void Entity::UpdateVelocity()
{
	if (m_horMoveDir != HOR_NONE)
	{
		if (m_velocity.x * m_horMoveDir < m_maxSpeed)
		{
			if (m_velocity.x * m_horMoveDir + m_horMoveDir * m_acceleration > m_maxSpeed)
				m_velocity.x = m_maxSpeed * m_horMoveDir;
			else
				m_velocity.x += m_horMoveDir * m_acceleration;
		}
	}
	else
	{
		if (m_velocity.x > 0.f)
		{
			if (m_velocity.x - m_acceleration < 0.f)
				m_velocity.x = 0.f;
			else
				m_velocity.x -= m_acceleration;
		}
		else if (m_velocity.x < 0.f)
		{
			if (m_velocity.x + m_acceleration > 0.f)
				m_velocity.x = 0.f;
			else
				m_velocity.x += m_acceleration;
		}
	}

	if (m_verMoveDir != VER_NONE)
	{
		if (m_velocity.y * m_verMoveDir < m_maxSpeed)
		{
			if (m_velocity.y * m_verMoveDir + m_verMoveDir * m_acceleration > m_maxSpeed)
				m_velocity.y = m_maxSpeed * m_verMoveDir;
			else
				m_velocity.y += m_verMoveDir * m_acceleration;
		}
	}
	else
	{
		if (m_velocity.y > 0.f)
		{
			if (m_velocity.y - m_acceleration < 0.f)
				m_velocity.y = 0.f;
			else
				m_velocity.y -= m_acceleration;
		}
		else if (m_velocity.y < 0.f)
		{
			if (m_velocity.y + m_acceleration > 0.f)
				m_velocity.y = 0.f;
			else
				m_velocity.y += m_acceleration;
		}
	}

}

void Entity::WallCollision(sf::FloatRect *walls)
{
	auto pos = m_pSprite->getPosition() - m_pSprite->getOrigin();

	sf::FloatRect movedHitbox(pos.x + m_hitbox.left, pos.y + m_hitbox.top, m_hitbox.width, m_hitbox.height);

	for (int i = 0; i < 4; ++i)
	{
		auto wall = walls[i];

		for (int i = 0; i < 4; ++i)
		{
			auto wall = walls[i];

			if (movedHitbox.intersects(wall))
			{
				switch (i)
				{
					case 0: // top
						if (m_velocity.y < 0.f)
							m_verCollMoveBlock = UP;
						break;
					case 1: // bottom
						if (m_velocity.y > 0.f)
							m_verCollMoveBlock = DOWN;
						break;
					case 2: // left
						if (m_velocity.x < 0.f)
							m_horCollMoveBlock = LEFT;
						break;
					case 3: // right
						if (m_velocity.x > 0.f)
							m_horCollMoveBlock = RIGHT;
						break;
				}
			}
		}
	}
}

bool Entity::CollideWithObject(Object *object, bool blockMove)
{
	auto thisPos = m_pSprite->getPosition() - m_pSprite->getOrigin();
	auto objectPos = object->GetPosition() - object->GetOrigin();
	auto objectHitbox = object->GetHitbox();

	auto thisHitbox = sf::FloatRect(thisPos.x + m_hitbox.left, thisPos.y + m_hitbox.top, m_hitbox.width, m_hitbox.height);
	objectHitbox = sf::FloatRect(objectPos.x + objectHitbox.left, objectPos.y + objectHitbox.top, objectHitbox.width, objectHitbox.height);

	if (blockMove)
	{
		bool isCollision = false;

		// left side
		if (sf::FloatRect(thisHitbox.left - m_maxSpeed, thisHitbox.top, 1.f, thisHitbox.height).intersects(objectHitbox))
		{
			isCollision = true;
			m_horCollMoveBlock = LEFT;

		}
		// right side
		else if (sf::FloatRect(thisHitbox.left + thisHitbox.width + m_maxSpeed, thisHitbox.top, 1.f, thisHitbox.height).intersects(objectHitbox))
		{
			isCollision = true;
			m_horCollMoveBlock = RIGHT;

		}

		// top side
		if (sf::FloatRect(thisHitbox.left, thisHitbox.top - m_maxSpeed, thisHitbox.width, 1.f).intersects(objectHitbox))
		{
			isCollision = true;
			m_verCollMoveBlock = UP;

		}
		// bottom side
		else if (sf::FloatRect(thisHitbox.left, thisHitbox.top + thisHitbox.height + m_maxSpeed, thisHitbox.width, 1.f).intersects(objectHitbox))
		{
			isCollision = true;
			m_verCollMoveBlock = DOWN;

		}

		return isCollision;
	}
	else
	{
		if (thisHitbox.intersects(objectHitbox))
			return true;
		else
			return false;
	}
}