#pragma once

#include "Room.h"

namespace sf
{
	class Sprite;
}

class BossRoom : public Room
{
private:
	std::shared_ptr<sf::Sprite>				m_pPodiumSprite;

public:
	BossRoom(Map *parent, const sf::Vector2i &pos);
	~BossRoom();

	void Pulse(float deltaTime) override;
	void RenderGame(sf::RenderWindow *window) override;
	void RenderHud(sf::RenderWindow *window) override;
	void Event(sf::Event *e) override;

	void RoomCleanEvent() override;
};

