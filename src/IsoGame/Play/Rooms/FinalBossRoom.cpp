#include "FinalBossRoom.h"

#include <Game.h>

#include <Play\Objects\Entities\Enemies\Bosses\FinalBoss.h>

FinalBossRoom::FinalBossRoom(Map *parent, const sf::Vector2i &pos)
	: Room(parent, pos)
{
	auto windowSize = sf::Vector2f(Game::Get()->GetWindowSize());

	m_pFinalBoss = new FinalBoss(windowSize / 2.f - sf::Vector2f(0.f, 50.f));

	AddObject(m_pFinalBoss);
	
	m_isRoomClear = false;
	m_specialRoom = true;
	m_roomType = Type::FINAL_BOSS;
	m_roomColor = sf::Color(0x792725FF);
}

FinalBossRoom::~FinalBossRoom()
{

}

void FinalBossRoom::Pulse(float deltaTime)
{
	Room::Pulse(deltaTime);
}

void FinalBossRoom::RenderGame(sf::RenderWindow *window)
{
	Room::RenderGame(window);
}

void FinalBossRoom::Event(sf::Event *e)
{
	Room::Event(e);
}
