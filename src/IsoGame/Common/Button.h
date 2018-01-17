#pragma once

#include <functional>
#include <memory>

#include <SFML\System\Vector2.hpp>
#include <SFML\System\Clock.hpp>

namespace sf
{
	class RenderWindow;
	class Event;
	class Text;
	class RectangleShape;
}

class Button
{
private:
	std::function<void()>					m_func;

	std::shared_ptr<sf::Text>				m_pText;
	std::shared_ptr<sf::RectangleShape>		m_pButtonShape;
	

	sf::Clock								m_animStart;
	int										m_animDelay;
	bool									m_enterAnim;
	int										m_animProc;

	char									m_alpha;

	char									m_highlighted;

public:
	Button(const sf::Vector2f &pos, const char *text, std::function<void()> toCall, bool enterAnim = true, int delay = 1500);
	~Button() = default;

	void Pulse(float deltaTime);
	void Render(sf::RenderWindow *window);
	bool Event(sf::Event *e);
};