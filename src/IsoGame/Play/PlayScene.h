#pragma once

#include <GameScene.h>

#include <memory>
#include <string>

#include <SFML\Graphics\Vertex.hpp>
#include <SFML\System\Clock.hpp>

#include <functional>

namespace sf
{
	class Text;
	class Sprite;
}

class Map;

class Player;

class PlayScene : public GameScene
{
private:
	std::shared_ptr<Map>				m_pMap;

	std::shared_ptr<Player>				m_pPlayer;

	std::shared_ptr<sf::Sprite>			m_pHearth;

	std::shared_ptr<sf::Text>			m_pStatsText;

	sf::Vertex							m_backgroundRect[4];

	int									m_dimness;

	// show info
	bool								m_showingInfo;
	bool								m_showingMsg;
	sf::Clock							m_showTime;
	int									m_timeout;
	std::shared_ptr<sf::Text>			m_pTitle;


	sf::Clock							m_fadeAnimStat;
	int									m_animSpeed;
	int									m_delay;
	bool								m_fadeInAnim;
	bool								m_fadeOutAnim;
	std::function<void()>				m_callAfterFade;


	sf::Clock							m_second;
	std::shared_ptr<sf::Text>			m_pScoreText;
	int64_t								m_score;


	bool								m_endingGame;
	bool								m_finished;

	static PlayScene					*s_pInst;

private:
	void FadeIn(int animSpeed = 5, int delay = 0, std::function<void()> callAfter = 0);
	void FadeOut(int animSpeed = 5, int delay = 0, std::function<void()> callAfter = 0);

public:
	PlayScene();
	~PlayScene() = default;

	void ShowInfo(const char *msg, int timeout);

	virtual void Pulse(float deltaTime) override;
	virtual void RenderGame(sf::RenderWindow *window) override;
	virtual void RenderHud(sf::RenderWindow *window) override;
	virtual void Event(sf::Event *e) override;

	void AddScore(int64_t score);
	void SubScore(int64_t score);

	void EndGame(bool finished);

	auto GetMap() { return m_pMap.get(); }

	static auto Get() { return s_pInst; }
};

