#include "Enemy.h"

#include <Common\Common.h>

#include <Play\PlayScene.h>
#include <Play\Objects\Entities\Bullet.h>
#include <Play\Objects\Entities\Player.h>
#include <Play\Rooms\Room.h>

#include <SFML\Graphics.hpp>

Enemy::Enemy(const sf::Vector2f &pos)
	: Entity(pos)
{
	m_acceleration = 0.2f;
	m_maxSpeed = 1.f;

	m_horMoveDir = HOR_NONE;
	m_verMoveDir = VER_NONE;

	m_invincible = false;

	m_objectType = Object::ENEMY;
	m_damageAnim = false;

	m_deathAnim = false;
	m_animAlpha = 100;

	m_scoreValue = 0;

	m_moving = true;


	m_particlesColor = sf::Color::Red;
}

Enemy::~Enemy()
{

}

void Enemy::DeathEvent()
{
	m_pParent->RemoveSafeObject(this);
}

bool Enemy::CollideWithObject(Object *object, bool blockMove)
{
	if (object->GetObjectType() == Object::BULLET && static_cast<Bullet*>(object)->GetBulletType() == Bullet::FRIENDLY)
	{
		if (Entity::CollideWithObject(object, false))
		{
			if (!m_invincible)
			{
				m_health -= int(static_cast<Bullet*>(object)->GetDamage());

				if (m_particleAnim)
					CreateParticles(pointDir(GetPosition(), object->GetPosition()));
				
				PlayScene::Get()->AddScore(10);

				if (m_health <= 0)
				{
					m_pParent->RemoveObject(object);
					m_deathAnim = true;
					PlayScene::Get()->AddScore(m_scoreValue);
					return true;
				}

				m_lastAnimChange.restart();
				m_damageAnim = true;
				m_lastSpriteColor = m_pSprite->getColor();
				m_pSprite->setColor(sf::Color(0xff6767FF));
			}

			m_pParent->RemoveObject(object);

			return true;
		}

		return false;
	}

	if (object->GetObjectType() == Object::PICKUP || object->GetObjectType() == Object::PLAYER)
		return false;



	return Entity::CollideWithObject(object, blockMove);
}

void Enemy::Pulse(float deltaTime)
{
	if (m_deathAnim)
	{
		m_animAlpha -= 15;

		if (m_animAlpha <= 0)
		{
			DeathEvent();
			return;
		}

		auto color = m_pSprite->getColor();
		color.a = (m_animAlpha * 255) / 100;
		m_pSprite->setColor(color);
	}

	if (m_damageAnim && m_lastAnimChange.getElapsedTime().asMilliseconds() > m_damageAnimDuration)
	{
		m_pSprite->setColor(m_lastSpriteColor);
		m_damageAnim = false;
	}

	if (m_moving)
	{
		auto newMoveDir = Directions::NONE;

		if (m_horMoveDir == LEFT)
			newMoveDir = Directions::Direction(newMoveDir + Directions::W);
		else if (m_horMoveDir == RIGHT)
			newMoveDir = Directions::Direction(newMoveDir + Directions::E);

		if (m_verMoveDir == UP)
			newMoveDir = Directions::Direction(newMoveDir + Directions::N);
		else if (m_verMoveDir == DOWN)
			newMoveDir = Directions::Direction(newMoveDir + Directions::S);

		if (newMoveDir == Directions::NONE)
			newMoveDir = Directions::S;

		SetTexture(newMoveDir);
	}

	UpdateVelocity();

	auto pos = m_pSprite->getPosition();
	if (m_velocity.x < 0.f && m_horCollMoveBlock != LEFT)
		pos.x += m_velocity.x * deltaTime;
	else if (m_velocity.x > 0.f && m_horCollMoveBlock != RIGHT)
		pos.x += m_velocity.x * deltaTime;

	if (m_velocity.y < 0.f && m_verCollMoveBlock != UP)
		pos.y += m_velocity.y * deltaTime;
	else if (m_velocity.y > 0.f && m_verCollMoveBlock != DOWN)
		pos.y += m_velocity.y * deltaTime;
	m_pSprite->setPosition(pos);

	m_zDepth = pos.y;

	Object::Pulse(deltaTime);
}

void Enemy::RenderGame(sf::RenderWindow *window)
{
	window->draw(*m_pSprite);

	Object::RenderGame(window);
}

void Enemy::Event(sf::Event *e)
{

}
