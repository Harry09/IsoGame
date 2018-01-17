#include "Map.h"

#include <Game.h>

#include <Common\ResourceMgr.h>
#include <Common\Random.h>

#include "PlayScene.h"

#include "Objects\Entities\Player.h"

#include "Rooms\Room.h"
#include "Rooms\BossRoom.h"
#include "Rooms\FinalBossRoom.h"
#include "Rooms\TreasureRoom.h"
#include "Rooms\ZombieRoom.h"
#include "Rooms\ShooterRoom.h"

#include <SFML\Graphics.hpp>

#include <conio.h>
#include <stdarg.h> 

//#define MAP_GEN_DEBUG

Map::Map(Player *player)
	: m_pPlayer(player)
{
	m_pCurrentRoom = nullptr;

	m_pRoomShape = std::make_shared<sf::RectangleShape>(sf::Vector2f(15.f, 7.5f));
	m_pRoomShape->setFillColor(sf::Color::Black);

	GenerateMap();

	/*for (auto room : m_rooms)
	{
		if (room)
		{
			if (room->GetRoomType() == Room::FINAL_BOSS)
				LoadRoom(room.get());
		}
	}*/

	m_isFinalBossOpen = false;

	Map::LoadRoom(m_rooms[0].get());
}

Map::~Map()
{
}


void Map::GenerateMap(int seed)
{
	Random random;

	if (seed != -1)
		random = Random(seed);

	int nAttempts = 0;

	while (true)
	{
		nAttempts++;

		m_mapRoom.clear();
		m_rooms.clear();

		m_nBossRooms = 3;
		m_nFinalBossRooms = 1;
		m_nTreasureRooms = 3;
		m_nEmptyRooms = 4;
		m_nZombieRooms = 5;
		m_nShooterRooms = 4;

		m_nRoomsToCreate = m_nRooms = GetNumberOfAvailableRooms();

		sf::Vector2i pos;

		printf("Generating map... Rooms: %d\n", m_nRooms);

		Generate(Directions::NONE, random, pos, 0);

		//printf("%d %d\n", m_rooms.size(), m_nRoomsToCreate);

		if (m_rooms.size() == m_nRoomsToCreate)
			break;
	}

	// set desc for portals
	for (auto& room : m_rooms)
	{
		if (room)
		{
			auto pos = room->GetPos();

			for (int num = 0; num < 4; ++num)
			{
				auto numDir = NumToDir(num);
				auto numPos = MoveInDir(pos, numDir);

				if (m_mapRoom.find(std::make_pair(numPos.x, numPos.y)) != m_mapRoom.end())
				{
					room->SetPortalDest(numDir, m_mapRoom[std::make_pair(numPos.x, numPos.y)]);
				}
				else
				{
					room->CreateMeteor(numDir);
				}
			}

			room->ClosePortals(false);

			switch (room->GetRoomType())
			{
				case Room::EMPTY:
				case Room::TREASURE:
					room->OpenPortals(false);
			}
		}
	}

	printf("Done in %d attempts!\n", nAttempts);
}

sf::Vector2i Map::MoveInDir(const sf::Vector2i &pos, Directions::Direction dir)
{
	sf::Vector2i newPos = pos;

	switch (dir)
	{
		case Directions::NW:
			newPos += sf::Vector2i(-1, -1);
			break;
		case Directions::NE:
			newPos += sf::Vector2i(1, -1);
			break;
		case Directions::SE:
			newPos += sf::Vector2i(1, 1);
			break;
		case Directions::SW:
			newPos += sf::Vector2i(-1, 1);
			break;
	}

	return newPos;
}

Directions::Direction Map::NumToDir(int num)
{
	switch (num)
	{
		case 0:
			return Directions::NW;
		case 1:
			return Directions::NE;
		case 2:
			return Directions::SE;
		case 3:
			return Directions::SW;
	}
	
	return Directions::NONE;
}

bool Map::IsNearToSpecialRoom(const sf::Vector2i &oldPos, const sf::Vector2i &newPos)
{
	bool isSpecialRoomNear = false;

	for (int i = 0; i < 4; ++i)
	{ 
		auto _newPos = MoveInDir(newPos, NumToDir(i));

		if (_newPos == oldPos)
			continue;

		if (m_mapRoom.find(std::make_pair(_newPos.x, _newPos.y)) != m_mapRoom.end())
		{
			if (m_mapRoom[std::make_pair(_newPos.x, _newPos.y)]->IsSpecialRoom())
				isSpecialRoomNear = true;
		}
	}

	return isSpecialRoomNear;
}

void Map::GenerateTree(Random &random, const sf::Vector2i &pos, int deepLevel, int min, int max)
{
	int nNewRooms = random.Get<int>(std::min(min, m_nRooms), std::min(max, m_nRooms));

	m_nRooms -= nNewRooms;
	
	PrintTab(deepLevel, "Generating tree: %d", nNewRooms);

	bool reserved[4] = { false };

	for (int i = 0; i < nNewRooms; ++i)
	{
		
		PrintTab(deepLevel, "New: %d", i);

		sf::Vector2i newRoomPos;
		Directions::Direction newRoomDir;

		int nAttempts = 20;

		while (true)
		{
			if (nAttempts <= 0)
				break;

			nAttempts--;

			int num = random.Get<int>(0, 3);

			if (reserved[num])
				continue;

			auto drawn = NumToDir(num);

			auto newPos = MoveInDir(pos, drawn);

			if (m_mapRoom.find(std::make_pair(newPos.x, newPos.y)) == m_mapRoom.end())
			{
				if (IsNearToSpecialRoom(pos, newRoomPos))
					continue;

				newRoomDir = drawn;
				newRoomPos = newPos;
				reserved[num] = true;
				break;
			}
		}

		if (nAttempts <= 0)
		{
			m_nRooms++;
			PrintTab(deepLevel, "Cannot %d", i);
			continue;
		}

		Generate(newRoomDir, random, newRoomPos, ++deepLevel, true);
	}

}

bool Map::Generate(Directions::Direction cameFrom, Random &random, const sf::Vector2i &pos, int deepLevel, bool reserved)
{
	if (!reserved)
	{
		if (m_nRooms <= 0)
			return false;
	}

	PrintTab(deepLevel, "Creating room... (%d)", m_nRooms);

	Room *room = nullptr;

	if (GetNumberOfAvailableRooms() == 0)
		return false;

	while (true)
	{
		int roomType = 1;

		if (pos != sf::Vector2i(0, 0))
			roomType = random.Get<int>(1, 6);

		bool created = false;

		switch (roomType)
		{
			case 1: // Empty Room
				if (m_nEmptyRooms > 0)
				{
					room = new Room(this, pos);
					m_nEmptyRooms--;
					created = true;
				} break;
			case 2: // Treasure Room
				if (m_nTreasureRooms > 0 && m_nRooms < m_nRoomsToCreate / 2)
				{
					room = new TreasureRoom(this, pos);
					m_nTreasureRooms--;
					created = true;
				} break;
			case 3: // Zombie Room
				if (m_nZombieRooms > 0)
				{
					room = new ZombieRoom(this, pos);
					m_nZombieRooms--;
					created = true;
				} break;
			case 4: // Boss Room
			{
				if (m_nBossRooms > 0 && m_nRooms < m_nRoomsToCreate / 3)
				{
					room = new BossRoom(this, pos);
					m_nBossRooms--;
					created = true;
				} break;
			}
			case 5: // Shooter Room
			{
				if (m_nShooterRooms > 0)
				{
					room = new ShooterRoom(this, pos);
					m_nShooterRooms--;
					created = true;
				}
			} break;
			case 6: // Final Boss
			{
				if (m_nFinalBossRooms > 0)
				{
					room = new FinalBossRoom(this, pos);
					m_nFinalBossRooms--;
					created = true;
				}
			} break;
		} 

		if (created)
			break;
	}

	
	PrintTab(deepLevel, "Created: %d on (%d, %d)", room->GetRoomType(), pos.x, pos.y);

	m_rooms.push_back(std::shared_ptr<Room>(room));
	m_mapRoom[std::make_pair(pos.x, pos.y)] = room;

	if (room->IsSpecialRoom())
	{
		
		PrintTab(deepLevel, "Special room: Return without childs");
		return true;
	}

	int generateNewTree = 2;

	if (cameFrom != Directions::NONE)
		generateNewTree = random.Get<int>(1, 4);

	if (generateNewTree == 2)
	{
		int min = 2;

		if (cameFrom == Directions::NONE)
		{
			min = 3;
		}

		GenerateTree(random, pos, ++deepLevel, min);

	}
	else
	{
		sf::Vector2i newRoomPos = MoveInDir(pos, cameFrom);

		
		PrintTab(deepLevel, "Next line");

		deepLevel++;

		if (IsNearToSpecialRoom(pos, newRoomPos))
			GenerateTree(random, pos, deepLevel);
		else
		{
			m_nRooms--;

			if (Generate(cameFrom, random, newRoomPos, deepLevel))
			{
				GenerateTree(random, pos, deepLevel);
			}
		}
	}

	return false;
}

void Map::PrintTab(int deepLevel, char * str, ...)
{
#ifdef MAP_GEN_DEBUG
	for (int i = 0; i <= deepLevel; ++i)
	{
		printf(" ");
	}

	char buffer[256];
	va_list args;
	va_start(args, str);
	vsnprintf(buffer, 256, str, args);
	va_end(args);

	char buffer2[512] = { 0 };

	sprintf(buffer2, "[%d] %s (R: %d, T: %d B: %d Z: %d E: %d", deepLevel, buffer, m_nRooms, m_nTreasureRooms, m_nBossRooms, m_nZombieRooms, m_nEmptyRooms);

	puts(buffer2);
#endif
}

int Map::GetNumberOfAvailableRooms()
{
	return m_nEmptyRooms + m_nTreasureRooms + m_nZombieRooms + m_nShooterRooms + m_nBossRooms + m_nFinalBossRooms;
}

void Map::LoadRoom(Room *room)
{
	if (room == nullptr)
		return;

	if (m_pCurrentRoom)
		m_pCurrentRoom->RemoveObject(m_pPlayer);

	m_pCurrentRoom = room;

	if (m_pCurrentRoom->GetVisibleStatus() != Room::SHOWN)
		PlayScene::Get()->AddScore(1000);

	m_pCurrentRoom->AddObject(m_pPlayer, false);
	m_pCurrentRoom->SetVisibleStatus(Room::SHOWN);

	auto setVisibity = [this] (Directions::Direction dir)
	{
		auto room =	m_pCurrentRoom->GetPortalDest(dir);

		if (room && room->GetVisibleStatus() == Room::HIDDEN)
			room->SetVisibleStatus(Room::HALF);
	};

	setVisibity(Directions::NE);
	setVisibity(Directions::NW);
	setVisibity(Directions::SE);
	setVisibity(Directions::SW);


	int notDiscoveredBosses = 0;

	for (auto room : m_rooms)
	{
		if (room)
		{
			if (room->GetRoomType() == Room::BOSS && room->GetVisibleStatus() != Room::SHOWN)
				notDiscoveredBosses++;
		}
	}

	if (notDiscoveredBosses == 0)
		m_isFinalBossOpen = true;
}

void Map::RenderHud(sf::RenderWindow *window)
{
	if (m_pCurrentRoom)
		m_pCurrentRoom->RenderHud(window);

	sf::Vector2f centerPos;
	centerPos.x = Game::Get()->GetWindowSize().x / 2.f;
	centerPos.y = (Game::Get()->GetWindowSize().y - Game::Get()->GetPlayAreaSize().y) / 2.f;
	float offset = 1.f;

	auto currentPos = m_pCurrentRoom->GetPos();

	for (auto& room : m_rooms)
	{
		if (room)
		{
			auto pos = room->GetPos() - currentPos;

			sf::Vector2f newPos;
			newPos.x = pos.x * (offset + m_pRoomShape->getSize().x) + 500.f;
			newPos.y = pos.y * (offset + m_pRoomShape->getSize().y);

			if ((centerPos + newPos).y > 130)
				continue;

			if (room.get() == m_pCurrentRoom)
				m_pRoomShape->setFillColor(sf::Color::White);
			else
			{
				if (room->GetVisibleStatus() == Room::SHOWN)
					m_pRoomShape->setFillColor(room->GetRoomColor());
				else if (room->GetVisibleStatus() == Room::HALF)
					m_pRoomShape->setFillColor(sf::Color(255, 255, 255, 50));
				else
					m_pRoomShape->setFillColor(sf::Color::Transparent);
			}
			m_pRoomShape->setPosition(centerPos + newPos);

			window->draw(*m_pRoomShape);
		}
	}
}

void Map::RenderGame(sf::RenderWindow *window)
{
	if (m_pCurrentRoom)
		m_pCurrentRoom->RenderGame(window);

}

void Map::Pulse(float deltaTime)
{
	if (m_pCurrentRoom)
		m_pCurrentRoom->Pulse(deltaTime);
}

void Map::Event(sf::Event *e)
{
	if (m_pCurrentRoom)
		m_pCurrentRoom->Event(e);

#ifdef _DEBUG
	if (e->type == sf::Event::KeyPressed)
	{
		switch (e->key.code)
		{
			case sf::Keyboard::G:
			{
				m_pCurrentRoom = nullptr;

				GenerateMap();
				Map::LoadRoom(m_rooms[0].get());
			} break;
			case sf::Keyboard::X:
			{
				for (auto room : m_rooms)
				{
					if (room)
					{
						room->SetVisibleStatus(Room::SHOWN);
					}
				}
			} break;
		}

	}


#endif
}
