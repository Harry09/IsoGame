#pragma once

#include <SFML\Graphics\Rect.hpp>
#include <SFML\Graphics\Color.hpp>

#include <memory>
#include <vector>
#include <map>

namespace sf
{
	class RenderWindow;
	class Event;
}

namespace Directions
{
	enum Direction;
}

class Map;
class Object;
class Wall;
class Portal;

class Room
{
public:
	enum Type
	{
		EMPTY,
		ENEMY,
		TREASURE,
		BOSS,
		FINAL_BOSS
	};

	enum VisibleStatus
	{
		HIDDEN,
		HALF,
		SHOWN
	};

protected:
	Map											*m_pParent;

	bool										m_specialRoom;

	sf::Vector2i								m_pos;

	Type										m_roomType;
	sf::Color									m_roomColor;

	std::vector<std::shared_ptr<Object>>		m_objects;

	std::vector<Object*>						m_renderQueue;

	std::vector<Object*>						m_objectsToRemove;

	VisibleStatus								m_visibleStatus;

	sf::FloatRect								m_wallHitbox[4];
	sf::FloatRect								m_backupWallHitbox[4];
	std::vector<std::shared_ptr<Wall>>			m_wall;

	bool										m_isRoomClear;

	std::map<Directions::Direction, Room*>		m_portalDest;


	std::string									m_roomPosText;

private:
	Portal *CreatePortal(Directions::Direction dir);
	void AddWall(Wall *wall);

	static bool ObjectComp(Object *obj1, Object *obj2);

public:
	Room(Map *parent, const sf::Vector2i &pos);
	~Room();

	void AddObject(Object *object, bool autoDelete = true);
	void RemoveObject(Object *object);

	void RemoveSafeObject(Object *object);

	void SetPortalDest(Directions::Direction portalPos, Room *desc);
	Room *GetPortalDest(Directions::Direction portalPos);

	void CreateMeteor(Directions::Direction pos);

	virtual void RoomCleanEvent();

	virtual void Pulse(float deltaTime);
	virtual void RenderGame(sf::RenderWindow *window);
	virtual void RenderHud(sf::RenderWindow *window);
	virtual void Event(sf::Event *e);

	void ClosePortals(bool anim = true);
	void OpenPortals(bool anim = true);

	void SetVisibleStatus(VisibleStatus visibleStatus) { m_visibleStatus = visibleStatus; }
	const auto& GetVisibleStatus() const { return m_visibleStatus; }

	auto GetParent() { return m_pParent; }

	auto& GetPos() const { return m_pos; }

	auto& GetPosAsString() const { return m_roomPosText; }
	
	auto GetRoomType() const { return m_roomType; }

	sf::FloatRect GetPlayZone();

	// special room can have only one connection
	bool IsSpecialRoom() const { return m_specialRoom; }

	const auto& GetRoomColor() const { return m_roomColor; }
};

