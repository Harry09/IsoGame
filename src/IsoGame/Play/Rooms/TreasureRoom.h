#pragma once

#include "Room.h"

namespace sf
{
	class Sprite;
}

class TreasureRoom : public Room
{
private:
	std::shared_ptr<sf::Sprite>		m_pPodiumSprite;

public:
	TreasureRoom(Map *parent, const sf::Vector2i &pos);
	~TreasureRoom();

	void RenderGame(sf::RenderWindow *window) override;
};

