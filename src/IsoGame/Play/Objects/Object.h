#pragma once


#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Rect.hpp>
#include <SFML\System\Clock.hpp>
#include <SFML\Graphics\Color.hpp>

#include <memory>
#include <map>
#include <string>

#include <Common\Directions.h>

namespace sf
{
	class RenderWindow;
	class Event;
	class Texture;
	class Sprite;
}

class Room;

class ParticleSystem;

class Object
{
public:
	enum ObjectType
	{
		NONE,
		WALL,
		PORTAL,
		PLAYER,
		ENEMY,
		BULLET,
		ROCKS,
		PICKUP
	};

	enum TypeDirection
	{
		FULL_DIRECTIONS, 
		HALF_DIRECTIONS // only NE, NW, SE, SW
	};

private:
	std::shared_ptr<ParticleSystem>						m_pParticles;
	sf::Clock											m_particlesCreateTime;
	static const int									m_particlesLifetime = 1000;

protected:
	Room												*m_pParent;

	bool												m_entity;
	
	std::shared_ptr<sf::Sprite>							m_pSprite;

	std::map<Directions::Direction, sf::Texture*>		m_textures;

	ObjectType											m_objectType;

	sf::FloatRect										m_hitbox;

	float												m_zDepth;


	bool												m_particleAnim;
	sf::Color											m_particlesColor;


protected:
	void LoadTexture(const std::string &textureName, Directions::Direction dir);
	void LoadTextures(const std::string &textureName, TypeDirection typeDir);
	void SetTexture(Directions::Direction direction, bool center = true);

public:
	Object(const sf::Vector2f &pos);
	~Object();

	virtual void Pulse(float deltaTime);
	virtual void RenderGame(sf::RenderWindow *window);
	virtual void RenderHud(sf::RenderWindow *window) { }
	virtual void Event(sf::Event *e) = 0;

	virtual void SetParent(Room *parent) { m_pParent = parent; }

	void CreateParticles(float dir);

	const sf::Vector2f& GetPosition() const;
	const sf::Vector2f& GetOrigin() const;

	auto& GetHitbox() { return m_hitbox; }

	auto& GetObjectType() { return m_objectType; }

	float GetZDepth() const { return m_zDepth; }

	bool IsEntity() const { return m_entity; }
};
