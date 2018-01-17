#include "ZombieRoom.h"

#include <Common\Random.h>

#include <Play\Objects\Entities\Enemies\Zombie.h>

ZombieRoom::ZombieRoom(Map *parent, const sf::Vector2i &pos)
	: Room(parent, pos)
{
	m_isRoomClear = false;
	m_roomType = Room::ENEMY;

	auto playZone = GetPlayZone();

	Random random;

	for (int i = 0; i < random.Get<int>(1, 4); ++i)
	{
		float x = random.Get<float>(playZone.left, playZone.left + playZone.width);
		float y = random.Get<float>(playZone.top, playZone.top + playZone.height);

		AddObject(new Zombie(sf::Vector2f(x, y)));
	}

	m_roomColor = sf::Color(0x66BB6AFF);
}

ZombieRoom::~ZombieRoom()
{
}
