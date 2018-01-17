#include <Game.h>


#include <Common\Directions.h>
#include <Common\Random.h>
#include <Common\ResourceMgr.h>

#include <End\EndScene.h>
#include <Menu\MenuScene.h>
#include <Play\PlayScene.h>

#include <Play\Objects\Entities\Player.h>

#include <SFML\Graphics.hpp>

Game *Game::s_pInst;

Game::Game()
{
	s_pInst = this;

	m_windowSize = sf::Vector2u(1280, 864);
	m_playAreaSize.x = m_windowSize.x;
	m_playAreaSize.y = m_windowSize.y - unsigned(m_windowSize.y * 0.167f);
}

Game::~Game()
{
}

bool Game::Init()
{
	Directions::init();

	sf::ContextSettings context;
	context.antialiasingLevel = 4;

	m_pWindow = std::make_shared<sf::RenderWindow>(sf::VideoMode(m_windowSize.x, m_windowSize.y), "IsoGame", sf::Style::Titlebar | sf::Style::Close, context);
	m_pWindow->setFramerateLimit(60);

	m_pResourceMgr = std::make_shared<ResourceMgr>();

	m_pGameView = std::make_shared<sf::View>(m_pWindow->getDefaultView());
	m_pGameView->setViewport(sf::FloatRect(0.f, 0.167f, 1.f, 1.f));

	m_pHudView = std::make_shared<sf::View>(m_pWindow->getDefaultView());
	
	m_pGameScene = nullptr;

	//RunPlayScene();
	RunMenuScene();

	return true;
}

void Game::Close()
{
	m_pWindow->close();
}

void Game::Run()
{
	while (m_pWindow->isOpen())
	{
		float deltaTime = m_gameClock.restart().asSeconds() * 90;

		sf::Event e;
		
		if (m_pWindow->pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
			{
				m_pWindow->close();
			}

			Event(&e);
		}

		Pulse(deltaTime);
		m_pWindow->clear(sf::Color(sf::Color::Black));
		m_pWindow->setView(*m_pGameView);
		RenderGame();
		m_pWindow->setView(*m_pHudView);
		RenderHud();
		m_pWindow->display();
	}
}

void Game::RunMenuScene()
{
	m_pGameScene.reset(new MenuScene());
}

void Game::RunPlayScene()
{
	m_pGameScene.reset(new PlayScene());
}

void Game::RunEndScene(int64_t score, bool finished)
{
	m_pGameScene.reset(new EndScene(score, finished));
}

void Game::Pulse(float deltaTime)
{
	if (m_pGameScene)
		m_pGameScene->Pulse(deltaTime);
}

void Game::RenderHud()
{
	if (m_pGameScene)
		m_pGameScene->RenderHud(m_pWindow.get());
}

void Game::RenderGame()
{
	if (m_pGameScene)
		m_pGameScene->RenderGame(m_pWindow.get());
}

void Game::Event(sf::Event *e)
{
	if (m_pGameScene)
		m_pGameScene->Event(e);
}
