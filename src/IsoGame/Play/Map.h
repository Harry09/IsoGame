#pragma once

#include <memory>
#include <vector>
#include <map>
#include <utility>

#include <SFML\System\Vector2.hpp>

namespace sf
{
	class RenderWindow;
	class Event;
	class RectangleShape; 
}

class Room;
class Player;
class Random;

namespace Directions
{
	enum Direction;
}

class Map
{
private:
	std::map<std::pair<int, int>, Room*>		m_mapRoom;
	std::vector<std::shared_ptr<Room>>			m_rooms;

	std::shared_ptr<sf::RectangleShape>			m_pRoomShape;

	Room										*m_pCurrentRoom;

	Player										*m_pPlayer;

	int											m_nRoomsToCreate;
	int											m_nRooms;

	int											m_nTreasureRooms;
	int											m_nBossRooms;
	int											m_nFinalBossRooms;
	int											m_nZombieRooms;
	int											m_nShooterRooms;
	int											m_nEmptyRooms;


	bool										m_isFinalBossOpen;

private:
	sf::Vector2i MoveInDir(const sf::Vector2i &pos, Directions::Direction dir);
	Directions::Direction NumToDir(int num);
	bool IsNearToSpecialRoom(const sf::Vector2i &oldPos, const sf::Vector2i &newPos);

	void GenerateTree(Random &random, const sf::Vector2i &pos, int deepLevel, int min = 2, int max = 4);

	bool Generate(Directions::Direction comeFrom, Random &random, const sf::Vector2i &pos, int deepLevel, bool reserved = false);

	void PrintTab(int deepLevel, char *str, ...);

	int GetNumberOfAvailableRooms();

public:
	Map(Player *player);
	~Map();
	
	// @seed if equal -1 then random
	void GenerateMap(int seed = -1);

	void LoadRoom(Room *room);

	void Pulse(float deltaTime);
	void RenderHud(sf::RenderWindow *window);
	void RenderGame(sf::RenderWindow *window);
	void Event(sf::Event *e);

	const bool IsFinalBossOpen() const { return m_isFinalBossOpen; }
};

