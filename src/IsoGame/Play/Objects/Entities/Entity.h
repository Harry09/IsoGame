#pragma once

#include <Play\Objects\Object.h>

#include <SFML\Graphics\Sprite.hpp>

class Entity : public Object
{
protected:
	enum HorDir
	{
		LEFT = -1,
		HOR_NONE,
		RIGHT
	};

	enum VerDir
	{
		UP = -1,
		VER_NONE,
		DOWN
	};

protected:
	sf::Vector2f				m_velocity;

	float						m_maxSpeed;

	Entity::HorDir				m_horMoveDir;
	Entity::VerDir				m_verMoveDir;

	Entity::HorDir				m_horCollMoveBlock;
	Entity::VerDir				m_verCollMoveBlock;

	float						m_acceleration;

public:
	Entity(const sf::Vector2f &pos);

	~Entity() = default;

	void UpdateVelocity();

	virtual void WallCollision(sf::FloatRect *walls);

	virtual bool CollideWithObject(Object *object, bool blockMove = true);
	
	void SetVelocity(const sf::Vector2f &velocity) { m_velocity = velocity; }
	const auto& GetVelocity() const { return m_velocity; }

	void ResetMoveBlocks()
	{
		m_horCollMoveBlock = HOR_NONE;
		m_verCollMoveBlock = VER_NONE;
	}

	virtual void Pulse(float deltaTime) = 0;
	virtual void RenderGame(sf::RenderWindow *window) = 0;
	virtual void Event(sf::Event *e) = 0;
};

