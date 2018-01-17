#pragma once

#include <SFML\System\Vector2.hpp>
#include <SFML\System\Clock.hpp>
#include <SFML\Graphics\Vertex.hpp>

#include <memory>

namespace sf
{
	class RenderWindow;
	class Event;
	class Texture;
	class View;
}

class ResourceMgr;
class Player;
class GameScene;

class Game
{
private:
	sf::Vector2u						m_windowSize;
	sf::Vector2u						m_playAreaSize;


	std::shared_ptr<sf::RenderWindow>	m_pWindow;
	std::shared_ptr<ResourceMgr>		m_pResourceMgr;

	std::unique_ptr<GameScene>			m_pGameScene;

	std::shared_ptr<sf::View>			m_pGameView;
	std::shared_ptr<sf::View>			m_pHudView;

	sf::Clock							m_gameClock;

	bool								m_loadingNewScene;

	static Game *s_pInst;

private:
	void Pulse(float deltaTime);
	void RenderHud();
	void RenderGame();
	void Event(sf::Event *e);

public:
	Game();
	~Game();

	bool Init();

	void Close();

	void Run();

	void RunMenuScene();
	void RunPlayScene();
	void RunEndScene(int64_t score, bool finished);

	auto& GetWindowSize()	const { return m_windowSize; }
	auto& GetPlayAreaSize() const { return m_playAreaSize; }

	auto GetWindow() { return m_pWindow.get(); }

	static Game *Get() { return s_pInst; }
};

