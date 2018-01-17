#include "Wall.h"

#include <Common\Random.h>

#include <SFML\Graphics.hpp>

char *Wall::TEXTURE_NAME = "meteorFullRound";
float Wall::SPRITE_SCALE = 0.8f;

Wall::Wall(const sf::Vector2f &pos, bool scaleWall)
	: Object(pos)
{
	Random random;

	int rand = random.Get<int>(0, 3);

	Directions::Direction dir;

	switch (rand)
	{
		case 0:
			dir = Directions::NE;
			break;
		case 1:
			dir = Directions::NW;
			break;
		case 2:
			dir = Directions::SE;
			break;
		case 3:
			dir = Directions::SW;
			break;
	}

	LoadTexture(TEXTURE_NAME, dir);
	SetTexture(dir);

	m_objectType = Object::WALL;

	if (scaleWall)
		m_pSprite->setScale(SPRITE_SCALE, SPRITE_SCALE);
}

Wall::~Wall()
{
}

void Wall::RenderGame(sf::RenderWindow *window)
{
	window->draw(*m_pSprite);
}
