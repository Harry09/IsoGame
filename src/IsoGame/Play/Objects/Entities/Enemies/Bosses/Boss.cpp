#include "Boss.h"

#include <Game.h>

#include <Play\Objects\Entities\Bullet.h>

#include <SFML\Graphics.hpp>

Boss::Boss(const sf::Vector2f &pos)
	: Enemy(pos)
{
	auto windowSize = sf::Vector2f(Game::Get()->GetWindowSize());
	auto playZone = sf::Vector2f(Game::Get()->GetPlayAreaSize());

	m_healthBarSize = sf::Vector2f(800.f, 20.f);
	m_pHealthBar = std::make_shared<sf::RectangleShape>(m_healthBarSize);

	float width = windowSize.x - m_healthBarSize.x;

	m_pHealthBar->setPosition(width / 2, windowSize.y - 50.f);
	m_pHealthBar->setFillColor(sf::Color(0xd40000FF));
	m_pHealthBar->setOrigin(0.f, 10.f);

	float marginSize = 4.f;

	m_pHealthBarBg = std::make_shared<sf::RectangleShape>(sf::Vector2f(m_healthBarSize.x + marginSize * 2, m_healthBarSize.y + marginSize * 2));
	m_pHealthBarBg->setOrigin(0.f, m_pHealthBarBg->getSize().y / 2);
	m_pHealthBarBg->setPosition(m_pHealthBar->getPosition() - sf::Vector2f(marginSize, 0.f));
	m_pHealthBarBg->setFillColor(sf::Color::Black);

	m_particlesColor = sf::Color(0xfcf8f3FF);
}

Boss::~Boss()
{
}

bool Boss::CollideWithObject(Object * object, bool blockMove)
{
	if (object->GetObjectType() == Object::BULLET && static_cast<Bullet*>(object)->GetBulletType() == Bullet::FRIENDLY)
	{
		if (Enemy::CollideWithObject(object, blockMove))
		{
			TakeHit();
			return true;
		}
	}

	return false;
}

void Boss::Pulse(float deltaTime)
{
	if (m_healthBarHitAnim && m_healthBarHitAnimStart.getElapsedTime().asMilliseconds() > 100)
	{
		m_pHealthBar->setFillColor(sf::Color(0xd40000FF));

		m_healthBarHitAnim = false;
	}

	Enemy::Pulse(deltaTime);
}

void Boss::RenderHud(sf::RenderWindow *window)
{
	window->draw(*m_pHealthBarBg);
	window->draw(*m_pHealthBar);
}

void Boss::SetHealthBarWidth(float proc)
{
	if (proc <= 0.f)
		m_pHealthBar->setSize(sf::Vector2f(0.f, m_healthBarSize.y));
	else
		m_pHealthBar->setSize(sf::Vector2f(m_healthBarSize.x * proc, m_healthBarSize.y));
}

void Boss::TakeHit()
{
	SetHealthBarWidth(float(m_health) / m_maxHealth);

	m_healthBarHitAnim = true;
	m_healthBarHitAnimStart.restart();
	m_pHealthBar->setFillColor(sf::Color(0x920000FF));
}
