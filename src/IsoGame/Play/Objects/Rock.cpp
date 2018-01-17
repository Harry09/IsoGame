#include "Rock.h"

#include <Common\Random.h>

#include <SFML\Graphics.hpp>

char *Rock::TEXTURE_NAME_1 = "rocksSmall";
char *Rock::TEXTURE_NAME_2 = "rocks";

Rock::Rock(const sf::Vector2f &pos)
	: Object(pos)
{
	m_objectType = Object::ROCKS;

	Random random;

	int texType = random.Get<int>(1, 2);
	
	auto dir = NumToDir(random.Get<int>(1, 4));

	if (texType == 1) // TEXTURE_NAME_1
	{
		switch (dir)
		{
			case Directions::NE:
				m_hitbox = sf::FloatRect(11, 16, 62, 52);
			case Directions::NW:
				m_hitbox = sf::FloatRect(4, 18, 74, 50);
			case Directions::SE:
				m_hitbox = sf::FloatRect(20, 20, 64, 56);
			case Directions::SW:
				m_hitbox = sf::FloatRect(19, 13, 72, 58);

		}
	}
	else if (texType == 2) // TEXTURE_NAME_2
	{
		m_hitbox = sf::FloatRect(20, 18, 72, 53);
	}

	LoadTexture(TEXTURE_NAME_1, dir);
	SetTexture(dir);

	m_hitsTaken = 0;

	m_particlesColor = sf::Color(0xe1a478FF);
}

Rock::~Rock()
{
}

void Rock::Pulse(float deltaTime)
{

	Object::Pulse(deltaTime);
}

void Rock::RenderGame(sf::RenderWindow *window)
{
	window->draw(*m_pSprite);

	Object::RenderGame(window);
}

void Rock::Event(sf::Event *e)
{
}

Directions::Direction Rock::NumToDir(int num)
{
	switch (num)
	{
		case 1:
			return Directions::NE;
		case 2:
			return Directions::NW;
		case 3:
			return Directions::SE;
		case 4:
			return Directions::SW;
	}

	return Directions::NONE;
}
