#include "TreasureRoom.h"

#include "Game.h"

#include <Common\Directions.h>
#include <Common\ResourceMgr.h>

#include <Play\Objects\Pickup.h>

#include <SFML\Graphics.hpp>

TreasureRoom::TreasureRoom(Map *parent, const sf::Vector2i &pos)
	: Room(parent, pos)
{
	auto areaSize = sf::Vector2f(Game::Get()->GetPlayAreaSize());

	auto tex = ResourceMgr::Get()->GetTexture("crater", Directions::NE);
	
	if (tex && tex->m_pTexture)
	{
		m_pPodiumSprite = std::make_shared<sf::Sprite>();
		m_pPodiumSprite->setTexture(*tex->m_pTexture);
		m_pPodiumSprite->setOrigin(sf::Vector2f(tex->m_pTexture->getSize()) / 2.f);
		m_pPodiumSprite->setPosition(areaSize / 2.f);
	}

	AddObject(new Pickup(areaSize / 2.f - sf::Vector2f(0.f, 22.f)));

	m_specialRoom = true;
	m_roomType = Type::TREASURE;
	m_roomColor = sf::Color(0xFFEE58FF);
}

TreasureRoom::~TreasureRoom()
{

}

void TreasureRoom::RenderGame(sf::RenderWindow *window)
{
	if (m_pPodiumSprite)
		window->draw(*m_pPodiumSprite);

	Room::RenderGame(window);
}
