#include "Room.h"

#include <Game.h>

#include <Common\ResourceMgr.h>
#include <Common\Random.h>
#include <Common\Common.h>

#include <Play\Map.h>
#include <Play\PlayScene.h>
#include <Play\Objects\Entities\Player.h>
#include <Play\Objects\Portal.h>
#include <Play\Objects\Wall.h>
#include <Play\Objects\Pickup.h>

#include <algorithm>

#include <SFML\Graphics.hpp>

Room::Room(Map *parent, const sf::Vector2i &pos)
	: m_pParent(parent), m_pos(pos)
{
	m_visibleStatus = HIDDEN;

	m_specialRoom = false;

	m_isRoomClear = true;

	m_roomPosText = "(" + std::to_string(m_pos.x) + ", " + std::to_string(m_pos.y) + ")";

	auto playAreaSize = Game::Get()->GetPlayAreaSize();

	auto meteorTexture = ResourceMgr::Get()->GetTexture(Wall::TEXTURE_NAME, Directions::NE);

	if (meteorTexture && meteorTexture->m_pTexture)
	{
		auto meteorSize = sf::Vector2f(meteorTexture->m_pTexture->getSize());

		sf::Vector2f meteorSizeScaled(meteorSize.x * Wall::SPRITE_SCALE, meteorSize.y * Wall::SPRITE_SCALE);

		float meteorOffsetX = 136.f;
		float meteorOffsetY = 170.f;

		// top
		for (int i = 0; i < 11; ++i)
		{
			sf::Vector2f pos;

			pos.x = meteorOffsetX * 1.25f + i * meteorSizeScaled.x;
			pos.y = meteorSizeScaled.y / 2 + 16.f;

			auto wall = new Wall(pos);
			AddObject(wall);
		}

		m_wallHitbox[0] = sf::FloatRect(meteorOffsetX * 1.10f, 0, 10.3f * meteorSizeScaled.x, meteorSizeScaled.y + 16.f);

		// bottom
		for (int i = 0; i < 11; ++i)
		{
			sf::Vector2f pos;

			pos.x = meteorOffsetX * 1.25f + i * meteorSizeScaled.x;
			pos.y = playAreaSize.y - meteorSizeScaled.y / 2;

			auto wall = new Wall(pos);
			AddObject(wall);
		}

		m_wallHitbox[1] = sf::FloatRect(meteorOffsetX* 1.10f, playAreaSize.y - meteorSizeScaled.y + 16.f, 10.4f * meteorSizeScaled.x, meteorSizeScaled.y + 16.f);

		// left
		for (int i = 0; i < 6; ++i)
		{
			sf::Vector2f pos;

			pos.x = meteorSizeScaled.x / 2;
			pos.y = meteorOffsetY + meteorSizeScaled.y / 1.2f * i;

			auto wall = new Wall(pos);
			AddObject(wall);
		}

		m_wallHitbox[2] = sf::FloatRect(0, meteorOffsetY, meteorSizeScaled.x, meteorSizeScaled.y * 4);


		// right
		for (int i = 0; i < 6; ++i)
		{
			sf::Vector2f pos;

			pos.x = playAreaSize.x - meteorSizeScaled.x / 2;
			pos.y = meteorOffsetY + meteorSizeScaled.y / 1.2f * i;

			auto wall = new Wall(pos);
			AddObject(wall);
		}

		m_wallHitbox[3] = sf::FloatRect(playAreaSize.x - meteorSizeScaled.x, meteorOffsetY, meteorSizeScaled.x, meteorSizeScaled.y * 4);

	}

	m_roomType = Type::EMPTY;
	m_roomColor = sf::Color(0xBDBDBDFF);
}

Room::~Room()
{
}

void Room::AddObject(Object *object, bool autoDelete)
{
	if (object == nullptr)
		return;

	object->SetParent(this);

	if (autoDelete)
		m_objects.push_back(std::shared_ptr<Object>(object));

	m_renderQueue.push_back(object);
}

void Room::RemoveObject(Object *object)
{
	for (int i = 0; i < m_objects.size(); ++i)
	{
		if (m_objects[i].get() == object)
		{
			m_objects.erase(m_objects.begin() + i);
		}
	}

	for (int i = 0; i < m_renderQueue.size(); ++i)
	{
		if (m_renderQueue[i] == object)
		{
			m_renderQueue.erase(m_renderQueue.begin() + i);
		}
	}
}

void Room::RemoveSafeObject(Object *object)
{
	m_objectsToRemove.push_back(object);
}

void Room::SetPortalDest(Directions::Direction portalPos, Room *desc)
{
	Portal *portal = nullptr;

	switch (portalPos)
	{
		case Directions::NE:
		{
			portal = CreatePortal(Directions::SW);
		} break;
		case Directions::NW:
		{
			portal = CreatePortal(Directions::SE);
		} break;
		case Directions::SE:
		{
			portal = CreatePortal(Directions::NW);
		} break;
		case Directions::SW:
		{
			portal = CreatePortal(Directions::NE);
		} break;
	}

	if (!portal)
		return;

	m_portalDest[portalPos] = desc;
	portal->SetDest(desc);
}

Room *Room::GetPortalDest(Directions::Direction portalPos)
{
	if (m_portalDest.find(portalPos) != m_portalDest.end())
		return m_portalDest[portalPos];

	return nullptr;
}

void Room::CreateMeteor(Directions::Direction pos)
{
	auto meteorTex = ResourceMgr::Get()->GetTexture(Wall::TEXTURE_NAME, Directions::NE);

	if (meteorTex && meteorTex->m_pTexture) 
	{
		auto meteorSize = meteorTex->m_pTexture->getSize();

		auto windowSize = Game::Get()->GetPlayAreaSize();

		sf::Vector2f realPos;

		switch (pos)
		{
			case Directions::NE:
			{
				realPos.x = windowSize.x - meteorSize.x / 2.f;
				realPos.y = meteorSize.y / 2.f;

				auto x = m_wallHitbox[0].left;
				m_wallHitbox[0].width = realPos.x - x;

				auto y = m_wallHitbox[3].top;
				auto endY = y + m_wallHitbox[3].height;
				m_wallHitbox[3].top = realPos.y;
				m_wallHitbox[3].height = endY - realPos.y;
			} break;
			case Directions::NW:
			{
				realPos = sf::Vector2f(meteorSize) / 2.f;

				auto x = m_wallHitbox[0].left;
				auto endX = x + m_wallHitbox[0].width;
				m_wallHitbox[0].left = realPos.x;
				m_wallHitbox[0].width = endX - realPos.x;

				auto y = m_wallHitbox[2].top;
				auto endY = y + m_wallHitbox[2].height;
				m_wallHitbox[2].top = realPos.y;
				m_wallHitbox[2].height = endY - realPos.y;
			} break;
			case Directions::SE:
			{
				realPos = sf::Vector2f(windowSize) - (sf::Vector2f(meteorSize) / 2.f);

				auto x = m_wallHitbox[1].left;
				m_wallHitbox[1].width = realPos.x - x;

				auto y = m_wallHitbox[3].top;
				m_wallHitbox[3].height = realPos.y - y;

			} break;
			case Directions::SW:
			{
				realPos.x = meteorSize.x / 2.f;
				realPos.y = windowSize.y - meteorSize.y / 2.f;

				auto y = m_wallHitbox[2].top;
				m_wallHitbox[2].height = realPos.y - y;

				auto x = m_wallHitbox[1].left;
				auto endX = x + m_wallHitbox[1].width;
				m_wallHitbox[1].left = realPos.x;
				m_wallHitbox[1].width = endX - realPos.x;

			} break;
		}

		AddObject(new Wall(realPos, false));
	}
}

void Room::RoomCleanEvent()
{
	Random random;

	if (random.Get<int>(0, 2) == 2)
	{
		auto playZone = sf::Vector2f(Game::Get()->GetPlayAreaSize());

		AddObject(new Pickup(playZone / 2.f));
	}

	OpenPortals();
}

void Room::Pulse(float deltaTime)
{
	for (int i = 0; i < m_renderQueue.size(); ++i)
	{
		auto object = m_renderQueue[i];

		if (object)
		{
			object->Pulse(deltaTime);

			if (object->GetObjectType() == Object::PLAYER)
			{
				// portal collision
				if (m_isRoomClear)
				{
					for (int j = 0; j < m_renderQueue.size(); ++j)
					{
						Portal *portal = static_cast<Portal*>(m_renderQueue[j]);

						if (portal->GetObjectType() == Object::PORTAL)
						{
							auto player = static_cast<Player*>(object);

							if (player->IsNearPortal(portal))
							{
								auto dest = portal->GetDest();

								if (dest)
								{
									if (dest->GetRoomType() == Room::FINAL_BOSS && !m_pParent->IsFinalBossOpen())
									{
										PlayScene::Get()->ShowInfo("First defeat all bosses to go there!", 2000);

										auto dir = pointDir(portal->GetPosition(), player->GetPosition());

										player->SetVelocity(lenDir(5.f, dir));

										break;
									}

									m_pParent->LoadRoom(dest);

									auto windowSize = Game::Get()->GetPlayAreaSize();

									auto degreeDir = pointDir(portal->GetPosition(), sf::Vector2f(windowSize.x / 2.f, windowSize.y / 2.f));

									auto newPos = object->GetPosition() + lenDir(1100.f, degreeDir);

									player->SetPos(newPos);

									break;
								}
							}
						}
					}
				}
			}

			if (object->IsEntity())
			{
				auto entity = static_cast<Entity*>(object);

				entity->ResetMoveBlocks();

				bool skip = false;

				for (int j = 0; j < m_renderQueue.size(); ++j)
				{
					auto collideObj = m_renderQueue[j];

					if (collideObj)
					{
						auto objectType = entity->GetObjectType();

						if (entity->CollideWithObject(collideObj))
						{
							if (objectType == Object::BULLET)
							{
								skip = true;
								break;
							}
						}
					}
				}

				if (skip)
					continue;

				entity->WallCollision(m_wallHitbox);
			}
		}
	}

	for (auto object : m_objectsToRemove)
	{
		RemoveObject(object);
	}

	m_objectsToRemove.clear();
}

void Room::RenderGame(sf::RenderWindow *window)
{
	std::sort(m_renderQueue.begin(), m_renderQueue.end(), Room::ObjectComp);

	int nEnemies = 0;

	for (auto& object : m_renderQueue)
	{
		if (object)
		{
			object->RenderGame(window);

			if (object->GetObjectType() == Object::ENEMY)
				nEnemies++;

		}
	}

	if (!m_isRoomClear)
	{
		if (nEnemies == 0)
		{
			RoomCleanEvent();
		}
	}
}

void Room::RenderHud(sf::RenderWindow *window)
{
	for (auto& object : m_renderQueue)
	{
		if (object)
			object->RenderHud(window);
	}
}

void Room::Event(sf::Event *e)
{
	for (auto& object : m_renderQueue)
	{
		if (object)
			object->Event(e);
	}

#ifdef _DEBUG
	if (e->type == sf::Event::KeyPressed && e->key.code == sf::Keyboard::R)
	{
		OpenPortals();
		m_isRoomClear = true;
	}
	else if (e->type == sf::Event::KeyPressed && e->key.code == sf::Keyboard::T)
	{
		ClosePortals();
		m_isRoomClear = false;
	}
#endif
}

Portal *Room::CreatePortal(Directions::Direction dir)
{
	auto windowSize = Game::Get()->GetPlayAreaSize();

	auto portalTexture = ResourceMgr::Get()->GetTexture(Portal::PURPLE_TEXTURE_NAME, dir);

	if (portalTexture && portalTexture->m_pTexture)
	{
		auto texSize = portalTexture->m_pTexture->getSize();

		sf::Vector2f pos;

		switch (dir)
		{
			case Directions::NW:
				pos.x = windowSize.x - texSize.y / 2.f;
				pos.y = windowSize.y - texSize.y / 1.5f;
				break;
			case Directions::NE:
				pos.x = texSize.y / 2.f;
				pos.y = windowSize.y - texSize.y / 1.5f;
				break;
			case Directions::SE:
				pos.x = texSize.y / 2.f;
				pos.y = texSize.y / 2.f;
				break;
			case Directions::SW:
				pos.x = windowSize.x - texSize.y / 2.f;
				pos.y = texSize.y / 2.f;
				break;
		}

		auto portal = new Portal(pos, dir);
		AddObject(portal);
		return portal;
	}

	return nullptr;
}

void Room::AddWall(Wall *wall)
{
	m_wall.push_back(std::shared_ptr<Wall>(wall));
}

bool Room::ObjectComp(Object * obj1, Object * obj2)
{
	if (obj1 == nullptr || obj2 == nullptr)
		return false;

	return obj1->GetZDepth() < obj2->GetZDepth();
}

void Room::ClosePortals(bool anim)
{
	for (int i = 0; i < 4; ++i)
	{
		m_backupWallHitbox[i] = m_wallHitbox[i];
	}

	auto meteorTexture = ResourceMgr::Get()->GetTexture(Wall::TEXTURE_NAME, Directions::NE);

	if (meteorTexture && meteorTexture->m_pTexture)
	{
		auto meteorSize = sf::Vector2f(meteorTexture->m_pTexture->getSize());
		sf::Vector2f meteorSizeScaled = sf::Vector2f(meteorSize.x * Wall::SPRITE_SCALE, meteorSize.y * Wall::SPRITE_SCALE);

		auto windowSize = sf::Vector2f(Game::Get()->GetPlayAreaSize());

		m_wallHitbox[0] = sf::FloatRect(0, 0, windowSize.x, meteorSizeScaled.y + 16.f);
		m_wallHitbox[1] = sf::FloatRect(0, windowSize.y - meteorSizeScaled.y + 16.f, windowSize.x, meteorSizeScaled.y + 16.f);
		m_wallHitbox[2] = sf::FloatRect(0, 0, meteorSizeScaled.x, windowSize.y);
		m_wallHitbox[3] = sf::FloatRect(windowSize.x - meteorSizeScaled.x, 0, meteorSizeScaled.x, windowSize.y);
	}

	for (auto& object : m_renderQueue)
	{
		if (object && object->GetObjectType() == Object::PORTAL)
		{
			reinterpret_cast<Portal*>(object)->Close(anim);
		}
	}
}

void Room::OpenPortals(bool anim)
{
	for (int i = 0; i < 4; ++i)
	{
		m_wallHitbox[i] = m_backupWallHitbox[i];
	}

	for (auto& object : m_renderQueue)
	{
		if (object && object->GetObjectType() == Object::PORTAL)
		{
			reinterpret_cast<Portal*>(object)->Open(anim);
		}
	}

	m_isRoomClear = true;
}

sf::FloatRect Room::GetPlayZone()
{
	sf::FloatRect rect;

	rect.left = m_wallHitbox[2].width;
	rect.top = m_wallHitbox[0].height;
	rect.width = m_wallHitbox[3].left - rect.left;
	rect.height = m_wallHitbox[1].top - rect.top;

	return rect;
}
