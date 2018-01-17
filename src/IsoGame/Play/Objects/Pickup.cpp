#define _USE_MATH_DEFINES

#include "Pickup.h"

#include "Entities\Player.h"

#include <Play\PlayScene.h>
#include <Play\Rooms\Room.h>

#include <Common\Random.h>

#include <SFML\Graphics.hpp>

#include <cmath>
#include <iomanip>
#include <sstream>

Pickup::Pickup(const sf::Vector2f &pos, Pickup::Type type)
	: Object(pos), m_startPos(pos)
{
	m_objectType = Object::PICKUP;

	LoadTextures("barrel", Object::HALF_DIRECTIONS);

	SetTexture(Directions::NE);

	m_floating = 0.f;

	m_texIndex = 0;

	m_hitbox = sf::FloatRect(m_pSprite->getTextureRect());

	if (type == NONE)
	{
		Random random;

		type = Pickup::Type(random.Get<int>(1, 4));
	}

	m_type = type;

	switch (type)
	{
		case HEALTH:
			m_pSprite->setColor(sf::Color(0xEF9A9AFF));
			break;
		case DAMAGE:
			m_pSprite->setColor(sf::Color(0x64DD17FF));
			break;
		case MOVE_SPEED:
			m_pSprite->setColor(sf::Color(0x64B5F6FF));
			break;
		case SHOOTING_SPEED:
			m_pSprite->setColor(sf::Color(0xFFFF00FF));
			break;
	}
}

void Pickup::Active(Player *player)
{
	if (!player)
		return;

	if (!m_pParent)
		return;

	std::stringstream msg;

	Random random;

	int positive = random.Get<int>(0, 3);

	float value = 0.f;

	if (m_pParent->IsSpecialRoom())
	{
		value = random.Get<float>(2.f, 5.f);
		PlayScene::Get()->AddScore(1000);
	}
	else
	{
		if (positive >= 1 || positive <= 3)
			value = random.Get<float>(0.f, 1.f);
		else if (positive == 0)
			value = -random.Get<float>(0.f, 1.f);

		PlayScene::Get()->AddScore(500);
	}

	msg << (value > 0 ? "+" : "") << std::setprecision(3);

	switch (m_type)
	{
		case HEALTH:
		{
			msg.str("");
			msg.clear();

			int health = 0; 
			
			if (m_pParent->IsSpecialRoom())
				health = int(floor(value));
			else
				health = 1;

			msg << "+" << float(health) / 2 << " health";

			if (player->m_health < Player::MAX_HEALTH)
			{
				if (player->m_health + health > Player::MAX_HEALTH)
					player->m_health = Player::MAX_HEALTH;
				else
					player->m_health += health;
			}

		} break;
		case DAMAGE:
			msg << value << " damage";
			player->m_bulletDamage += value;
			break;
		case MOVE_SPEED:
			msg << value / 10.f << " move speed";
			player->m_moveSpeed += value / 10.f;
			break;
		case SHOOTING_SPEED:
			msg << value / 10.f << " shooting speed";
			player->m_shootingSpeed += value / 10.f;
			break;
	}

	static_cast<PlayScene*>(PlayScene::Get())->ShowInfo(msg.str().c_str(), 1000);

}

void Pickup::Pulse(float deltaTime)
{
	if (m_lastIndexChange.getElapsedTime().asMilliseconds() > 100)
	{
		m_texIndex++;

		if (m_texIndex == 2)
			m_texIndex = 0;

		switch (m_texIndex)
		{
			case 0:
				SetTexture(Directions::NE);
				break;
			case 1:
				SetTexture(Directions::NW);
				break;
		}

		m_lastIndexChange.restart();
	}

	m_floating += 0.05f * deltaTime;

	if (m_floating >= M_PI * 2)
		m_floating = 0.f;

	m_pSprite->setPosition(m_startPos.x, m_startPos.y - sin(m_floating) * 10);
}

void Pickup::RenderGame(sf::RenderWindow *window)
{
	window->draw(*m_pSprite);
}

void Pickup::Event(sf::Event *e)
{
}
