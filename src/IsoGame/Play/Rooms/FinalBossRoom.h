#pragma once

#include "Room.h"

class FinalBoss;

class FinalBossRoom : public Room
{
private:
	FinalBoss *m_pFinalBoss;

public:
	FinalBossRoom(Map *parent, const sf::Vector2i &pos);
	~FinalBossRoom();

	void Pulse(float deltaTime) override;
	void RenderGame(sf::RenderWindow *window) override;
	void Event(sf::Event *e) override;

	void RoomCleanEvent() override {};
};

