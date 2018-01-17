#pragma once

#include "Room.h"

class ZombieRoom : public Room
{
public:
	ZombieRoom(Map *parent, const sf::Vector2i &pos);
	~ZombieRoom();
};

