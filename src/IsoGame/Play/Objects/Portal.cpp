#include "Portal.h"

#include <Common\ParticleSystem.h>
#include <Common\ResourceMgr.h>

#include <Play\PlayScene.h>
#include <Play\Map.h>
#include <Play\Rooms\Room.h>

#include <SFML\Graphics.hpp>

char *Portal::PURPLE_TEXTURE_NAME = "portals\\portalPurple";
char *Portal::RED_TEXTURE_NAME = "portals\\portalRed";
char *Portal::GOLD_TEXTURE_NAME = "portals\\portalGold";
char *Portal::GRAY_TEXTURE_NAME = "portals\\portalMono";

Portal::Portal(const sf::Vector2f &pos, Directions::Direction dir)
	: Object(pos), m_portalDir(dir)
{
	auto tex = ResourceMgr::Get()->GetTexture(GRAY_TEXTURE_NAME, dir);

	if (tex && tex->m_pTexture)
	{
		m_pClosedPotal = std::make_shared<sf::Sprite>(*tex->m_pTexture);
		m_pClosedPotal->setOrigin(sf::Vector2f(tex->m_pTexture->getSize()) / 2.f);
		m_pClosedPotal->setPosition(pos);
	}

	m_objectType = Object::PORTAL;

	m_pDest = nullptr;

	m_isClosed = false;

	m_particles = std::make_shared<ParticleSystem>(1000, sf::seconds(1));
	m_particles->SetEmitterPos(pos);
	m_particles->SetColor(sf::Color(0x845c9fFF));
	m_particles->SetMinSpeed(5);
	m_particles->SetMaxSpeed(10);
	m_particles->SetSize(0.2f);

	m_alpha = 0;
	m_closingAnim = false;
	m_openingAnim = false;
}

Portal::~Portal()
{
}

void Portal::Pulse(float deltaTime)
{
	if (m_openingAnim)
	{
		if (m_alpha > 0)
			m_alpha -= 10;

		if (m_alpha <= 0)
		{
			m_openingAnim = false;
			m_isClosed = false;
		}

		auto color = m_pClosedPotal->getColor();
		color.a = m_alpha;
		m_pClosedPotal->setColor(color);
	}

	if (m_closingAnim)
	{
		if (m_alpha < 250)
			m_alpha += 10;

		if (m_alpha == 250)
		{
			m_closingAnim = false;
			m_isClosed = true;

		}

		auto color = m_pClosedPotal->getColor();
		color.a = m_alpha;
		m_pClosedPotal->setColor(color);
	}

	m_particles->Pulse(deltaTime);
}

void Portal::RenderGame(sf::RenderWindow *window)
{
	window->draw(*m_pSprite);

	if (m_isClosed)
		window->draw(*m_pClosedPotal);
	else
		m_particles->Render(window);
}

void Portal::Event(sf::Event *e)
{
}

void Portal::SetDest(Room *room)
{
	m_pDest = room;

	switch (room->GetRoomType())
	{
		case Room::EMPTY:
		case Room::ENEMY:
			LoadTexture(PURPLE_TEXTURE_NAME, m_portalDir);
			m_openColor = sf::Color::White;
			break;
		case Room::TREASURE:
			LoadTexture(GOLD_TEXTURE_NAME, m_portalDir);
			m_openColor = sf::Color::Yellow;
			break;
		case Room::BOSS:
			LoadTexture(RED_TEXTURE_NAME, m_portalDir);
			m_openColor = sf::Color::Red;
			break;
		case Room::FINAL_BOSS:
			LoadTexture(GRAY_TEXTURE_NAME, m_portalDir);
			m_pSprite->setColor(sf::Color::Black);
			m_openColor = sf::Color::Black;
			break;
	}

	SetTexture(m_portalDir);

	if (m_openColor != sf::Color::White)
		m_particles->SetColor(m_openColor);
}

void Portal::Open(bool anim)
{
	if (anim)
	{
		m_isClosed = true;
		m_openingAnim = true;
	}
	else
	{
		m_isClosed = false;
		m_alpha = 0;
	}
}

void Portal::Close(bool anim)
{

	if (anim)
	{
		m_isClosed = false;
		m_closingAnim = true;
	}
	else
	{
		m_isClosed = true;
		m_alpha = 250;
	}
}
