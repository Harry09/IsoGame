#include "ShooterRoom.h"

#include <Common\Random.h>

#include <Play\Objects\Entities\Enemies\Shooter.h>
#include <Play\Objects\Rock.h>

ShooterRoom::ShooterRoom(Map *parent, const sf::Vector2i &pos)
	: Room(parent, pos)
{
	m_isRoomClear = false;
	m_roomType = Room::ENEMY;

	auto playZone = GetPlayZone();

	Random random;

	for (int i = 0; i < random.Get<int>(1, 3); ++i)
	{
		float x = random.Get<float>(playZone.left, playZone.left + playZone.width);
		float y = random.Get<float>(playZone.top, playZone.top + playZone.height);

		AddObject(new Shooter(sf::Vector2f(x, y)));
	}


	for (int i = 0; i < random.Get<int>(4, 6); ++i)
	{
		float x = random.Get<float>(playZone.left, playZone.left + playZone.width);
		float y = random.Get<float>(playZone.top, playZone.top + playZone.height);

		AddObject(new Rock(sf::Vector2f(x, y)));
	}

	m_roomColor = sf::Color(0x5C6BC0FF);
}


ShooterRoom::~ShooterRoom()
{
}
