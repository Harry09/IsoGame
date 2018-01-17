#include "MenuScene.h"

#include <Game.h>

#include <Common\Button.h>
#include <Common\ResourceMgr.h>

#include <SFML\Graphics.hpp>

#ifdef _WIN32
#include <Windows.h>
#endif

MenuScene::MenuScene()
{
	auto windowSize = sf::Vector2f(Game::Get()->GetWindowSize());

	auto font = ResourceMgr::Get()->GetFont();

	if (font)
	{
		m_pTitle = std::make_shared<sf::Text>();
		m_pTitle->setFont(*font);
		m_pTitle->setString("The Fate of IsoGuy");
		m_pTitle->setCharacterSize(100);
		m_pTitle->setPosition( sf::Vector2f(windowSize.x / 2.f - m_pTitle->getLocalBounds().width / 2.f, windowSize.y / 10.f));
	}

	int i = 0;

	m_pPlayButton = std::make_shared<Button>(sf::Vector2f(windowSize.x / 2.f, windowSize.y / 2.5f + 150 * i++), "Play", MenuScene::PlayButton, false);
	m_pInfoButton = std::make_shared<Button>(sf::Vector2f(windowSize.x / 2.f, windowSize.y / 2.5f + 150 * i++), "Info", MenuScene::InfoButton, false);
	m_pExitButton = std::make_shared<Button>(sf::Vector2f(windowSize.x / 2.f, windowSize.y / 2.5f + 150 * i++), "Exit", MenuScene::ExitButton, false);
}

void MenuScene::Pulse(float deltaTime)
{
}

void MenuScene::RenderGame(sf::RenderWindow *window)
{
}

void MenuScene::RenderHud(sf::RenderWindow *window)
{
	if (m_pTitle)
		window->draw(*m_pTitle);

	if (m_pPlayButton)
		m_pPlayButton->Render(window);

	if (m_pInfoButton)
		m_pInfoButton->Render(window);

	if (m_pExitButton)
		m_pExitButton->Render(window);
}

void MenuScene::Event(sf::Event *e)
{
	if (e->type == sf::Event::KeyPressed && e->key.code == sf::Keyboard::Escape)
		Game::Get()->Close();

	if (m_pPlayButton)
	{
		if (m_pPlayButton->Event(e))
			return;
	}

	if (m_pInfoButton)
		m_pInfoButton->Event(e);

	if (m_pExitButton)
		m_pExitButton->Event(e);
}

void MenuScene::PlayButton()
{
	Game::Get()->RunPlayScene();
}

void MenuScene::InfoButton()
{
#ifdef _WIN32
	MessageBoxA(Game::Get()->GetWindow()->getSystemHandle(), "The Fate of IsoGuy\nCode: Piotr Krupa\nGame art: Kenney (kenney.nl)\nInspiration: The Binding of Isaac by Edmund McMillen", "Info", MB_ICONINFORMATION);
#endif
}

void MenuScene::ExitButton()
{
	Game::Get()->Close();
}
