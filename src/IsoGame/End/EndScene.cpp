#include "EndScene.h"

#include <Common\Button.h>
#include <Common\ResourceMgr.h>

#include <Game.h>

#include <SFML\Graphics.hpp>

EndScene::EndScene(int64_t score, bool finished)
{
	auto windowSize = sf::Vector2f(Game::Get()->GetWindowSize());

	auto font = ResourceMgr::Get()->GetFont();

	if (font)
	{
		m_pTitle = std::make_shared<sf::Text>();
		m_pTitle->setFont(*font);
		std::string text;

		if (finished)
			text = "You won!";
		else
			text = "You lose!";

		m_pTitle->setString(text);
		m_pTitle->setCharacterSize(100);

		auto localBounds = m_pTitle->getLocalBounds();

		m_pTitle->setPosition(windowSize / 2.f - sf::Vector2f(localBounds.width / 2, localBounds.height * 3.f));

		m_pScore = std::make_shared<sf::Text>();
		m_pScore->setFont(*font);
		m_pScore->setString("Score: " + std::to_string(score));
		m_pScore->setCharacterSize(81);

		localBounds = m_pScore->getLocalBounds();
		m_pScore->setPosition(windowSize.x / 2.f - localBounds.width / 2.f, m_pTitle->getPosition().y + m_pTitle->getCharacterSize());
	}

	m_pPlayAgainButton = std::make_shared<Button>(sf::Vector2f(windowSize.x / 3.0f, windowSize.y / 1.3f), "Play again", EndScene::PlayAgainButton);
	m_pExitButton = std::make_shared<Button>(sf::Vector2f(windowSize.x / 1.5f, windowSize.y / 1.3f), "Exit", EndScene::ExitButton);
}

void EndScene::Pulse(float deltaTime)
{
	if (m_pPlayAgainButton)
	{
		m_pPlayAgainButton->Pulse(deltaTime);
	}

	if (m_pExitButton)
	{
		m_pExitButton->Pulse(deltaTime);
	}
}

void EndScene::RenderGame(sf::RenderWindow *window)
{
}

void EndScene::RenderHud(sf::RenderWindow *window)
{
	if (m_pTitle)
	{
		window->draw(*m_pTitle);
	}

	if (m_pScore)
	{
		window->draw(*m_pScore);
	}

	if (m_pPlayAgainButton)
	{
		m_pPlayAgainButton->Render(window);
	}

	if (m_pExitButton)
	{
		m_pExitButton->Render(window);
	}
}

void EndScene::Event(sf::Event *e)
{
	if (e->type == sf::Event::KeyPressed && e->key.code == sf::Keyboard::Escape)
	{
		Game::Get()->RunMenuScene();
		return;
	}

	if (m_pPlayAgainButton)
	{
		if (m_pPlayAgainButton->Event(e))
			return;
	}

	if (m_pExitButton)
	{
		if (m_pExitButton->Event(e))
			return;
	}
}

void EndScene::PlayAgainButton()
{
	Game::Get()->RunPlayScene();
}

void EndScene::ExitButton()
{
	Game::Get()->RunMenuScene();
}
