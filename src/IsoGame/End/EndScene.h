#pragma once

#include <GameScene.h>

#include <memory>

namespace sf
{
	class Text;
}

class Button;

class EndScene : public GameScene
{
private:
	std::shared_ptr<sf::Text>		m_pTitle;
	std::shared_ptr<sf::Text>		m_pScore;

	std::shared_ptr<Button>			m_pPlayAgainButton;
	std::shared_ptr<Button>			m_pExitButton;

private:
	static void PlayAgainButton();
	static void ExitButton();

public:
	EndScene(int64_t score, bool finished);
	~EndScene() = default;

	void Pulse(float deltaTime) override;
	void RenderGame(sf::RenderWindow * window) override;
	void RenderHud(sf::RenderWindow * window) override;
	void Event(sf::Event * e) override;
};

