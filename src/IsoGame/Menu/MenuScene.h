#pragma once

#include <GameScene.h>

#include <memory>

namespace sf
{
	class Text;
}

class Button; 

class MenuScene : public GameScene
{
private:
	std::shared_ptr<sf::Text>		m_pTitle;

	std::shared_ptr<Button>			m_pPlayButton;
	std::shared_ptr<Button>			m_pInfoButton;
	std::shared_ptr<Button>			m_pExitButton;

private:
	static void PlayButton();
	static void InfoButton();
	static void ExitButton();

public:
	MenuScene();
	~MenuScene() = default;

	void Pulse(float deltaTime) override;
	void RenderGame(sf::RenderWindow *window) override;
	void RenderHud(sf::RenderWindow *window) override;
	void Event(sf::Event *e) override;
};

