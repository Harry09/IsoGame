#include "Button.h"

#include <Common\ResourceMgr.h>

#include <SFML\Graphics.hpp>

Button::Button(const sf::Vector2f &pos, const char *text, std::function<void()> toCall, bool enterAnim, int delay)
	: m_func(toCall)
{
	auto font = ResourceMgr::Get()->GetFont();

	m_alpha = 50;
	m_highlighted = 100;

	if (font)
	{
		m_pButtonShape = std::make_shared<sf::RectangleShape>(sf::Vector2f(250.f, 60.f));
		m_pButtonShape->setFillColor(sf::Color(255, 255, 255, enterAnim ? 0 : 50));
		m_pButtonShape->setPosition(pos);
		m_pButtonShape->setOrigin(m_pButtonShape->getSize() / 2.f);

		m_pText = std::make_shared<sf::Text>();
		m_pText->setFont(*font);
		m_pText->setString(text);
		m_pText->setCharacterSize(40);

		auto localBounds = m_pText->getLocalBounds();

		m_pText->setOrigin(localBounds.left + localBounds.width / 2.f, localBounds.top + localBounds.height / 2.f);
		m_pText->setPosition(m_pButtonShape->getPosition());
		m_pText->setFillColor(sf::Color(255, 255, 255, enterAnim ? 0 : 255));
	}

	m_animStart.restart();
	m_animDelay = delay;
	m_enterAnim = enterAnim;
	m_animProc = 0;
}

void Button::Pulse(float deltaTime)
{
	if (m_enterAnim && m_animStart.getElapsedTime().asMilliseconds() > m_animDelay)
	{
		m_animProc+=2;

		if (m_animProc == 100)
			m_enterAnim = false;

		m_pButtonShape->setFillColor(sf::Color(255, 255, 255, (unsigned char)((m_animProc * 50.f) / 100.f)));
		m_pText->setFillColor(sf::Color(255, 255, 255, (unsigned char)((m_animProc * 255.f) / 100.f)));
	}
}

void Button::Render(sf::RenderWindow *window)
{
	if (m_pButtonShape)
		window->draw(*m_pButtonShape);

	if (m_pText)
		window->draw(*m_pText);
}

bool Button::Event(sf::Event *e)
{
	switch (e->type)
	{
		case sf::Event::MouseButtonPressed:
		{
			if (e->mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2f mousePos = sf::Vector2f(float(e->mouseButton.x), float(e->mouseButton.y));

				sf::FloatRect hitbox = sf::FloatRect(m_pButtonShape->getPosition() - m_pButtonShape->getOrigin(), m_pButtonShape->getSize());

				if (hitbox.contains(mousePos))
				{
					if (m_func)
					{
						m_func();
						return true;
					}
				}
			}

		} break;

		case sf::Event::MouseMoved:
		{
			if (!m_enterAnim)
			{
				sf::Vector2f mousePos = sf::Vector2f(float(e->mouseMove.x), float(e->mouseMove.y));

				sf::FloatRect hitbox = sf::FloatRect(m_pButtonShape->getPosition() - m_pButtonShape->getOrigin(), m_pButtonShape->getSize());

				if (hitbox.contains(mousePos))
				{
					m_pButtonShape->setFillColor(sf::Color(255, 255, 255, m_highlighted));
				}
				else
				{
					m_pButtonShape->setFillColor(sf::Color(255, 255, 255, m_alpha));
				}
			}
		} break;
	}

	return false;
}
