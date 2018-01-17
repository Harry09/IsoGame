#pragma once

namespace sf
{
	class RenderWindow;
	class Event;
}

class GameScene
{
public:
	GameScene() = default;
	virtual ~GameScene() = default;

	virtual void Pulse(float deltaTime) = 0;
	virtual void RenderGame(sf::RenderWindow *window) = 0;
	virtual void RenderHud(sf::RenderWindow *window) = 0;
	virtual void Event(sf::Event *e) = 0;
};

