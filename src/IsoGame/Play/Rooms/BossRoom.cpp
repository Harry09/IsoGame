#include "BossRoom.h"

#include <Game.h>

#include <Play\Objects\Pickup.h>
#include <Play\Objects\Entities\Enemies\Bosses\Boss1.h>

#include <Common\ResourceMgr.h>

#include <SFML\Graphics.hpp>

BossRoom::BossRoom(Map *parent, const sf::Vector2i &pos)
	: Room(parent, pos)
{
	m_isRoomClear = false;
	m_specialRoom = true;
	m_roomType = Type::BOSS;
	m_roomColor = sf::Color(0xEF5350FF);

	auto playZone = sf::Vector2f(Game::Get()->GetPlayAreaSize());
	AddObject(new Boss1(playZone / 2.f));


	auto tex = ResourceMgr::Get()->GetTexture("crater", Directions::NE);

	if (tex && tex->m_pTexture)
	{
		m_pPodiumSprite = std::make_shared<sf::Sprite>();
		m_pPodiumSprite->setTexture(*tex->m_pTexture);
		m_pPodiumSprite->setOrigin(sf::Vector2f(tex->m_pTexture->getSize()) / 2.f);
		m_pPodiumSprite->setPosition(playZone.x / 2, playZone.y / 1.5f);
	}
}

BossRoom::~BossRoom()
{
}

void BossRoom::Pulse(float deltaTime)
{
	Room::Pulse(deltaTime);
}

void BossRoom::RenderGame(sf::RenderWindow *window)
{
	if (m_isRoomClear)
	{
		window->draw(*m_pPodiumSprite);
	}

	Room::RenderGame(window);
}

void BossRoom::RenderHud(sf::RenderWindow *window)
{
	Room::RenderHud(window);
}

void BossRoom::Event(sf::Event *e)
{
	Room::Event(e);
}

void BossRoom::RoomCleanEvent()
{
	auto playZone = sf::Vector2f(Game::Get()->GetPlayAreaSize());
	AddObject(new Pickup(sf::Vector2f(playZone.x / 2, playZone.y / 1.5f) - sf::Vector2f(0.f, 22.f)));

	OpenPortals();
}

