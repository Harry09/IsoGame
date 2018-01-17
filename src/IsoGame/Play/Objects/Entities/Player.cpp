#define _USE_MATH_DEFINES

#include "Player.h"

#include <Game.h>

#include "Bullet.h"

#include <Common\Common.h>
#include <Common\Random.h>

#include <Play\PlayScene.h>
#include <Play\Objects\Portal.h>
#include <Play\Objects\Pickup.h>
#include <Play\Rooms\Room.h>

#include <SFML\Graphics.hpp>

Player *Player::s_pInst;

Player::Player(const sf::Vector2f &pos)
	: Entity(pos)
{
	m_invincible = false;

	s_pInst = this;

	m_acceleration = 0.2f;
	m_maxSpeed = 2.f;

	LoadTextures("astronaut", TypeDirection::FULL_DIRECTIONS);

	SetTexture(Directions::S);

	m_horMoveDir = HOR_NONE;
	m_verMoveDir = VER_NONE;

	m_horShotDir = HOR_NONE;
	m_verShotDir = VER_NONE;

	m_bulletSpeed = 5.f;
	m_shoting = false;

	m_resetDir = false;

	m_shotDir = Directions::S;

	m_hitbox = sf::FloatRect(0, 30, 52, 29);

	m_objectType = Object::PLAYER;

	m_damageAnim = false;
	m_damageAnimIndex = 0;

	m_deathAnim = false;
	m_deathAnimStage = 0;
	m_sin = 0.f;

	Random random;

	m_particlesColor = sf::Color::Red;

	// player stats
	m_health = MAX_HEALTH;
	m_bulletDamage = random.Get<float>(3.f, 6.f);//5.f;
	m_shootingSpeed = random.Get<float>(0.8f, 1.3f);//1.f;
	m_moveSpeed = random.Get<float>(0.8f, 1.3f);//1.f;
}

Player::~Player()
{
}

bool Player::CollideWithObject(Object *object, bool blockMove)
{
	if (object)
	{
		if (object->GetObjectType() == Object::ROCKS)
		{
			return Entity::CollideWithObject(object, true);
		}
		else if (object->GetObjectType() == Object::PICKUP)
		{
			if (Entity::CollideWithObject(object, true))
			{
				static_cast<Pickup*>(object)->Active(this);
				m_pParent->RemoveSafeObject(object);
				return true;
			}
		}
		if (object->GetObjectType() == Object::ENEMY && !m_damageAnim)
		{
			if (Entity::CollideWithObject(object, false))
			{
				CreateParticles(pointDir(GetPosition(), object->GetPosition()));

				m_velocity = lenDir(4.f, pointDir(object->GetPosition(), GetPosition()));

				TakeDamage();
				return true;
			}
		}
		else if (object->GetObjectType() == Object::BULLET && static_cast<Bullet*>(object)->GetBulletType() == Bullet::UNFRIENDLY && !m_damageAnim)
		{
			if (Entity::CollideWithObject(object, false))
			{
				CreateParticles(pointDir(GetPosition(), object->GetPosition()));

				m_velocity = lenDir(4.f, pointDir(object->GetPosition(), GetPosition()));

				TakeDamage();

				m_pParent->RemoveSafeObject(object);

				return true;
			}
		}

		return false;
	}

	return Entity::CollideWithObject(object, blockMove);
}

void Player::Pulse(float deltaTime)
{
	UpdateVelocity();

	auto velocity = m_velocity * m_moveSpeed;

	auto pos = m_pSprite->getPosition();

	if (velocity.x < 0.f && m_horCollMoveBlock != LEFT)
		pos.x += velocity.x * deltaTime;
	else if (velocity.x > 0.f && m_horCollMoveBlock != RIGHT)
		pos.x += velocity.x * deltaTime;

	if (velocity.y < 0.f && m_verCollMoveBlock != UP)
		pos.y += velocity.y * deltaTime;
	else if (velocity.y > 0.f && m_verCollMoveBlock != DOWN)
		pos.y += velocity.y * deltaTime;

	m_pSprite->setPosition(pos);

	m_zDepth = pos.y;

	if (m_health <= 0)
		m_deathAnim = true;

	if (m_deathAnim)
	{
		m_shoting = false;
		m_moveSpeed = 0;
		m_damageAnim = false;
		m_pSprite->setColor(sf::Color::White);

		if (m_deathAnimStage == 0)
		{
			SetTexture(Directions::S);

			auto texSize = sf::Vector2f(m_pSprite->getTexture()->getSize());
			auto oldOrigin = m_pSprite->getOrigin();
			auto newOrigin = sf::Vector2f(texSize.x / 2, texSize.y - 8.f);

			m_pSprite->setOrigin(newOrigin);

			auto pos = m_pSprite->getPosition();
			
			m_pSprite->setPosition(pos + sf::Vector2f(0.f, newOrigin.y - oldOrigin.y));

			m_deathAnimStage = 1;

			PlayScene::Get()->EndGame(false);
		}
		else if (m_deathAnimStage == 1)
		{
			float dir = m_pSprite->getRotation();

			if (dir < 90)
				dir += 5;

			if (dir == 90)
			{
				m_deathAnimStage = 2;
				auto texSize = sf::Vector2f(m_pSprite->getTexture()->getSize());

				m_ghostPos = m_pSprite->getPosition() + sf::Vector2f(texSize.x / 3.f, 0.f);
			}
			m_pSprite->setRotation(dir);
		}
		else if (m_deathAnimStage == 2)
		{
			m_ghostPos -= sf::Vector2f(0.f, 3.f) * deltaTime;

			m_sin += 0.1f;

			if (m_sin > M_PI * 2)
				m_sin = 0.f;
		}
	}

	if (!m_deathAnim && m_damageAnim && m_lastAnimIndexChange.getElapsedTime().asMilliseconds() > m_indexTime)
	{
		switch (m_damageAnimIndex)
		{
			case 5:
				m_damageAnim = false;
			case 1:
			case 3:
				m_pSprite->setColor(sf::Color::White);
				break;
			case 2:
			case 4:
				m_pSprite->setColor(sf::Color::Transparent);
				break;
		}
		m_damageAnimIndex++;
		m_lastAnimIndexChange.restart();
	}

	if (m_shoting && m_lastShotTime.getElapsedTime().asMilliseconds() > m_shotDelay / m_shootingSpeed)
	{
		auto bullet = new Bullet(Bullet::FRIENDLY, m_pSprite->getPosition(), sf::Vector2f(m_bulletSpeed * m_horShotDir + velocity.x / 1.25f, m_bulletSpeed * m_verShotDir + velocity.y / 1.25f));
		bullet->SetDamage(m_bulletDamage);
		m_pParent->AddObject(bullet);

		m_lastShotTime.restart();
	}

	if (!m_deathAnim && m_resetDir && m_resetDirTime.getElapsedTime().asMilliseconds() > 500)
	{
		m_shotDir = Directions::S;
		m_resetDir = false;
		SetTexture(m_shotDir);
	}

	Object::Pulse(deltaTime);
}

void Player::RenderGame(sf::RenderWindow *window)
{
	window->draw(*m_pSprite);

	if (m_deathAnim)
	{
		if (m_deathAnimStage == 2)
		{
			auto oldDir = m_pSprite->getRotation();
			auto oldPos = m_pSprite->getPosition();

			m_pSprite->setRotation(0);
			m_pSprite->setPosition(m_ghostPos + sf::Vector2f(sin(m_sin) * 20, 0.f));
			m_pSprite->setColor(sf::Color(100, 100, 100, 100));

			window->draw(*m_pSprite);

			m_pSprite->setRotation(oldDir);
			m_pSprite->setPosition(oldPos);
			m_pSprite->setColor(sf::Color::White);
		}
	}

	Object::RenderGame(window);
}

void Player::Event(sf::Event *e)
{
	if (e->type == sf::Event::KeyPressed)
	{
		switch (e->key.code)
		{
			case sf::Keyboard::W:
			{
					m_verMoveDir = UP;
			} break;
			case sf::Keyboard::S:
			{
					m_verMoveDir = DOWN;
			} break;
			case sf::Keyboard::A:
			{
					m_horMoveDir = LEFT;
			} break;
			case sf::Keyboard::D:
			{
					m_horMoveDir = RIGHT;
			} break;
			case sf::Keyboard::Up:
			{
				m_verShotDir = UP;
			} break;
			case sf::Keyboard::Down:
			{
				m_verShotDir = DOWN;
			} break;
			case sf::Keyboard::Left:
			{
				m_horShotDir = LEFT;
			} break;
			case sf::Keyboard::Right:
			{
				m_horShotDir = RIGHT;
			} break;

#ifdef _DEBUG
			case sf::Keyboard::Q:
				m_invincible = !m_invincible;
			break;
			case sf::Keyboard::F:
				m_bulletDamage = 20.f;
				m_shootingSpeed = 3.f;
				m_moveSpeed = 2.f;
			break;
#endif
		}
	}
	else if (e->type == sf::Event::KeyReleased)
	{
		switch (e->key.code)
		{
			case sf::Keyboard::W:
			{
				if (m_verMoveDir == UP)
					m_verMoveDir = VER_NONE;
			} break;
			case sf::Keyboard::S:
			{
				if (m_verMoveDir == DOWN)
					m_verMoveDir = VER_NONE;
			} break;
			case sf::Keyboard::A:
			{
				if (m_horMoveDir == LEFT)
					m_horMoveDir = HOR_NONE;
			} break;
			case sf::Keyboard::D:
			{
				if (m_horMoveDir == RIGHT)
					m_horMoveDir = HOR_NONE;
			} break;
			case sf::Keyboard::Up:
			{
				if (m_verShotDir == UP)
					m_verShotDir = VER_NONE;
			} break;
			case sf::Keyboard::Down:
			{
				if (m_verShotDir == DOWN)
					m_verShotDir = VER_NONE;
			} break;
			case sf::Keyboard::Left:
			{
				if (m_horShotDir == LEFT)
					m_horShotDir = HOR_NONE;
			} break;
			case sf::Keyboard::Right:
			{
				if (m_horShotDir == RIGHT)
					m_horShotDir = HOR_NONE;
			} break;
		}
	}

	if (!m_deathAnim)
	{
		auto newShotDir = Directions::NONE;

		m_resetDir = false;

		if (m_horShotDir == LEFT)
			newShotDir = Directions::Direction(newShotDir + Directions::W);
		else if (m_horShotDir == RIGHT)
			newShotDir = Directions::Direction(newShotDir + Directions::E);

		if (m_verShotDir == UP)
			newShotDir = Directions::Direction(newShotDir + Directions::N);
		else if (m_verShotDir == DOWN)
			newShotDir = Directions::Direction(newShotDir + Directions::S);

		if (m_horShotDir != HOR_NONE || m_verShotDir != VER_NONE)
			m_shoting = true;
		else
			m_shoting = false;

		if (newShotDir == Directions::NONE && m_shotDir != newShotDir)
		{
			m_resetDir = true;
			m_resetDirTime.restart();
		}
		else
			m_shotDir = newShotDir;

		SetTexture(m_shotDir);
	}
}

void Player::SetPos(sf::Vector2f &pos)
{
	m_pSprite->setPosition(pos);
}

bool Player::IsNearPortal(Portal *portal)
{
	if (lengthToPoint(GetPosition(), portal->GetPosition()) < MIN_LEN_TO_PORTAL)
		return true;

	return false;
}

void Player::TakeDamage()
{
	if (!m_deathAnim && !m_invincible)
	{
		m_damageAnim = true;
		m_damageAnimIndex = 0;
		m_lastAnimIndexChange.restart();

		PlayScene::Get()->SubScore(50);

		m_pSprite->setColor(sf::Color::Transparent);

		m_health--;
	}
}