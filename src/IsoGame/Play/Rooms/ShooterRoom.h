#pragma once
#include "Room.h"
class ShooterRoom : public Room
{
public:
	ShooterRoom(Map *parent, const sf::Vector2i &pos);
	~ShooterRoom();
};

