#pragma once

#include <Play\Objects\Entities\Enemies\Enemy.h>

namespace sf
{
	class RectangleShape;
}

class Boss : public Enemy
{
private:
	bool									m_healthBarHitAnim;
	sf::Clock								m_healthBarHitAnimStart;

	sf::Vector2f							m_healthBarSize;
	std::shared_ptr<sf::RectangleShape>		m_pHealthBar;
	std::shared_ptr<sf::RectangleShape>		m_pHealthBarBg;

protected:
	int										m_maxHealth;

public:
	Boss(const sf::Vector2f &pos);
	~Boss();

	bool CollideWithObject(Object *object, bool blockMove = true) override;

	void Pulse(float deltaTime) override;
	void RenderHud(sf::RenderWindow *window) override;

	void SetHealthBarWidth(float proc);

	void TakeHit();
};

