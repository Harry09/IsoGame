#include "Bullet.h"

#include <Game.h>

#include <Common\Common.h>

#include <Play\Objects\Rock.h>
#include <Play\Rooms\Room.h>

#include <SFML\Graphics.hpp>

Bullet::Bullet(Type bulletType, const sf::Vector2f &startPos, const sf::Vector2f &velocity)
	: Entity(startPos), m_bulletType(bulletType)
{
	m_velocity = velocity;

	m_direction = pointDir(m_startPos, m_startPos + m_velocity);

	Init();
}

Bullet::Bullet(Type bulletType, const sf::Vector2f &startPos, float dir, float speed)
	: Entity(startPos), m_bulletType(bulletType), m_direction(dir)
{
	m_velocity = lenDir(speed, dir);

	Init();
}

Bullet::~Bullet()
{
}

void Bullet::Init()
{
	LoadTexture("laserSabelOff", Directions::NE);
	m_pSprite->setRotation(degreeToSFMLRotate(m_direction - 90));
	SetTexture(Directions::NE);

	auto textureSize = sf::Vector2f(m_pSprite->getTexture()->getSize());

	m_hitbox = sf::FloatRect(0.f, 0.f, textureSize.x, textureSize.y);

	if (m_bulletType == Type::UNFRIENDLY)
		m_pSprite->setColor(sf::Color::Red);

	m_objectType = Object::BULLET;


	// when bullet is created in wall is instant deleting so I did it
	m_liveTicks = 0;
	m_inWall = false;
}

void Bullet::Pulse(float deltaTime)
{
	auto pos = m_pSprite->getPosition();
	pos.x += m_velocity.x * deltaTime;
	pos.y += m_velocity.y * deltaTime;
	m_pSprite->setPosition(pos);

	m_zDepth = pos.y;

	m_liveTicks++;
}

void Bullet::RenderGame(sf::RenderWindow *window)
{
	window->draw(*m_pSprite);
}

void Bullet::WallCollision(sf::FloatRect *walls)
{
	auto pos = m_pSprite->getPosition() - m_pSprite->getOrigin();

	for (int i = 0; i < 4; ++i)
	{
		auto wall = walls[i];

		if (pos.x < wall.left + wall.width &&
			pos.x + m_pSprite->getTexture()->getSize().x > wall.left &&
			pos.y < wall.top + wall.height &&
			pos.y + m_pSprite->getTexture()->getSize().y > wall.top)
		{
			if (m_liveTicks <= 1 || m_inWall)
			{
				m_inWall = true;
				return;
			}

			m_pParent->RemoveObject(this);
			return;
		}
		else
		{
			if (m_inWall)
				m_inWall = false;
		}
	}

	static auto playArea = Game::Get()->GetPlayAreaSize();

	if (pos.x < 0.f ||
		pos.x > playArea.x ||
		pos.y < 0.f ||
		pos.y > playArea.y)
	{
		m_pParent->RemoveObject(this);
	}
}

bool Bullet::CollideWithObject(Object *object, bool blockMove)
{
	if (object && object->GetObjectType() == Object::ROCKS)
	{
		auto rock = static_cast<Rock*>(object);

		if (Entity::CollideWithObject(rock, false))
		{
			rock->TakeHit();

			rock->CreateParticles(pointDir(GetPosition(), rock->GetPosition()));

			if (rock->GetTakenHits() > 3)
			{
				m_pParent->RemoveSafeObject(rock);
			}

			m_pParent->RemoveObject(this);
			return true;
		}
	}
	else if (object && object->GetObjectType() == Object::BULLET && static_cast<Bullet*>(object)->GetBulletType() != GetBulletType())
	{
		if (Entity::CollideWithObject(object, false))
		{
			m_pParent->RemoveSafeObject(this);
			m_pParent->RemoveSafeObject(object);
			return true;
		}
	}

	return false;
}
