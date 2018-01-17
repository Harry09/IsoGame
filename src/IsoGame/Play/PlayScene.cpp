#include "PlayScene.h"


#include <Common\ResourceMgr.h>
#include <Common\Directions.h>

#include <Game.h>

#include <Play\Map.h>
#include <Play\Objects\Entities\Player.h>

#include <SFML\Graphics.hpp>

#include <iomanip>
#include <sstream>

#ifdef _WIN32
#include <Windows.h>
#endif

PlayScene *PlayScene::s_pInst;

PlayScene::PlayScene()
{
	s_pInst = this;

	auto windowSize = Game::Get()->GetPlayAreaSize();
	sf::Color bgColor(225, 164, 120);

	m_backgroundRect[0] = sf::Vertex(sf::Vector2f(0.f, 0.f), bgColor);
	m_backgroundRect[1] = sf::Vertex(sf::Vector2f(float(windowSize.x), 0.f), bgColor);
	m_backgroundRect[2] = sf::Vertex(sf::Vector2f(float(windowSize.x), float(windowSize.y)), bgColor);
	m_backgroundRect[3] = sf::Vertex(sf::Vector2f(0.f, float(windowSize.y)), bgColor);

	m_pPlayer = std::make_shared<Player>(sf::Vector2f(windowSize.x / 2.f, windowSize.y / 2.f));

	m_pMap = std::make_shared<Map>(m_pPlayer.get());

	auto texture = ResourceMgr::Get()->GetTexture("astronaut", Directions::SE);

	if (texture && texture->m_pTexture)
	{
		m_pHearth = std::make_shared<sf::Sprite>(*texture->m_pTexture);
		m_pHearth->setScale(sf::Vector2f(0.7f, 0.7f));
	}

	auto font = ResourceMgr::Get()->GetFont();

	if (font)
	{
		m_pTitle = std::make_shared<sf::Text>();
		m_pTitle->setFont(*font);
		m_pTitle->setCharacterSize(60);

		m_pStatsText = std::make_shared<sf::Text>();
		m_pStatsText->setFont(*font);
		m_pStatsText->setCharacterSize(35);

		m_pScoreText = std::make_shared<sf::Text>();
		m_pScoreText->setFont(*font);
		m_pScoreText->setCharacterSize(25);
		m_pScoreText->setPosition(30.f, 85.f);
		m_score = 1000;
		AddScore(0);
	}

	m_showingInfo = false;

	m_fadeInAnim = false;
	m_fadeOutAnim = false;

	m_endingGame = false;

	m_finished = false;

	FadeIn(3, 300, [this] () { ShowInfo("Move: W/S/A/D\nAttack: Arrows", 3000); });
}

void PlayScene::FadeIn(int animSpeed, int delay, std::function<void()> callAfter)
{
	m_fadeInAnim = true;
	m_dimness = 255;
	m_callAfterFade = callAfter;

	m_animSpeed = animSpeed;

	m_delay = delay;
	m_fadeAnimStat.restart();
}

void PlayScene::FadeOut(int animSpeed, int delay, std::function<void()> callAfter)
{
	m_fadeOutAnim = true;
	m_dimness = 0;
	m_callAfterFade = callAfter;

	m_animSpeed = animSpeed;

	m_delay = delay;
	m_fadeAnimStat.restart();
}

void PlayScene::ShowInfo(const char *msg, int timeout)
{
	m_showingMsg = false;
	m_showingInfo = true;
	m_timeout = timeout;
	m_dimness = 0;

	m_pTitle->setString(msg);

	auto windowSize = Game::Get()->GetPlayAreaSize();

	m_pTitle->setPosition(windowSize.x / 2 - m_pTitle->getLocalBounds().width / 2, windowSize.y / 2 + m_pTitle->getLocalBounds().height / 2.f);
}

void PlayScene::Pulse(float deltaTime)
{
	if (m_pMap)
		m_pMap->Pulse(deltaTime);

	if (m_second.getElapsedTime().asMilliseconds() > 1000)
	{
		SubScore(5);
		m_second.restart();
	}

	if (m_fadeInAnim && m_fadeAnimStat.getElapsedTime().asMilliseconds() > m_delay)
	{
		if (m_dimness > 0)
			m_dimness -= m_animSpeed;

		if (m_dimness <= 0)
		{
			m_fadeInAnim = false;

			if (m_callAfterFade)
			{
				m_callAfterFade();
			}
		}
	}

	if (m_fadeOutAnim && m_fadeAnimStat.getElapsedTime().asMilliseconds() > m_delay)
	{
		if (m_dimness < 255)
			m_dimness += m_animSpeed;

		if (m_dimness >= 255)
		{
			m_fadeOutAnim = false;

			if (m_callAfterFade)
			{
				m_callAfterFade();
			}
		}
	}

	if (m_showingInfo)
	{
		if (!m_showingMsg && m_dimness < 150)
			m_dimness += 5;

		if (m_dimness == 150)
		{
			m_showTime.restart();
			m_showingMsg = true;
			m_dimness = 149;
		}

		if (m_showingMsg && m_showTime.getElapsedTime().asMilliseconds() > m_timeout)
		{
			m_dimness -= 5;

			if (m_dimness <= 0)
			{
				m_showingInfo = false;
				m_showingMsg = false;
			}
		}
	}
}

void PlayScene::RenderGame(sf::RenderWindow *window)
{
	window->draw(&m_backgroundRect[0], 4, sf::PrimitiveType::Quads);

	if (m_pMap)
		m_pMap->RenderGame(window);
}

void PlayScene::RenderHud(sf::RenderWindow *window)
{
	// render health
	if (m_pHearth)
	{
		auto health = m_pPlayer->GetHealth();

		int i = 0;

		for (; i < health - 1; i += 2)
		{
			m_pHearth->setPosition(sf::Vector2f(16.f + i * 24.f, 16.f));
			window->draw(*m_pHearth);
		}

		if (health % 2 != 0)
		{
			auto texRect = m_pHearth->getTextureRect();

			auto newTexRect = texRect;
			newTexRect.width /= 2;

			m_pHearth->setTextureRect(newTexRect);
			m_pHearth->setPosition(sf::Vector2f(16.f + i * 24.f, 16.f));
			window->draw(*m_pHearth);
			m_pHearth->setTextureRect(texRect);
		}
	}

	// render score
	if (m_pScoreText)
	{
		window->draw(*m_pScoreText);
	}

	// render stats
	if (m_pStatsText)
	{
		std::stringstream text;

		text << "Damage: " << std::setprecision(5) << m_pPlayer->GetBulletDamage() << "\n";
		text << "Move speed: " << std::setprecision(5)<< m_pPlayer->GetMoveSpeed() << "\n";
		text << "Shooting speed: " << std::setprecision(5) << m_pPlayer->GetShootingSpeed() << "\n";

		m_pStatsText->setString(text.str());
		m_pStatsText->setPosition(300.f, 8.f);
		window->draw(*m_pStatsText);
	}

	if (m_pMap)
		m_pMap->RenderHud(window);

	if (m_showingInfo || m_fadeInAnim || m_fadeOutAnim)
	{
		auto windowSize = sf::Vector2f(Game::Get()->GetWindowSize());

		if (m_dimness < 0)
			m_dimness = 0;

		if (m_dimness > 255)
			m_dimness = 255;

		sf::Color bgColor(0, 0, 0, m_dimness);

		sf::Vertex quad[4];
		quad[0] = sf::Vertex(sf::Vector2f(0.f, 0.f), bgColor);
		quad[1] = sf::Vertex(sf::Vector2f(windowSize.x, 0.f), bgColor);
		quad[2] = sf::Vertex(sf::Vector2f(windowSize.x, windowSize.y), bgColor);
		quad[3] = sf::Vertex(sf::Vector2f(0.f, windowSize.y), bgColor);
		window->draw(&quad[0], 4, sf::PrimitiveType::Quads);
	}

	if (m_showingInfo)
	{
		m_pTitle->setFillColor(sf::Color(255, 255, 255, (m_dimness * 255) / 200));
		window->draw(*m_pTitle);
	}
}

void PlayScene::Event(sf::Event *e)
{
	if (e->type == sf::Event::KeyPressed && e->key.code == sf::Keyboard::Escape)
	{
#ifdef _WIN32
		auto result = MessageBoxA(Game::Get()->GetWindow()->getSystemHandle(), "Are you sure you want to go to the menu?", "IsoQuestion", MB_ICONQUESTION | MB_YESNO);

		if (result == IDYES)
		{
			Game::Get()->RunMenuScene();

			return;
		}
#else
		Game::Get()->RunMenuScene();
		return;
#endif
	}

	if (m_pMap)
		m_pMap->Event(e);
}

void PlayScene::AddScore(int64_t score)
{
	if (score < 0)
		m_score -= score;
	else
		m_score += score;

	if (m_pScoreText)
		m_pScoreText->setString("Score: " + std::to_string(m_score));
}

void PlayScene::SubScore(int64_t score)
{
	if (m_score )

	if (score < 0)
		m_score += score;
	else
		m_score -= score;

	if (m_pScoreText)
		m_pScoreText->setString("Score: " + std::to_string(m_score));
}

void PlayScene::EndGame(bool finished)
{
	if (!m_endingGame)
	{
		m_finished = finished;

		FadeOut(2, 100, [this] () { Game::Get()->RunEndScene(m_score, m_finished); });
		m_endingGame = true;
	}
}
