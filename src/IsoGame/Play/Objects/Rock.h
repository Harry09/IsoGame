#pragma once

#include "Object.h"

class Rock : public Object
{
private:
	static char *TEXTURE_NAME_1;
	static char *TEXTURE_NAME_2;

	int m_hitsTaken;


private:
	Directions::Direction NumToDir(int num);

public:
	Rock(const sf::Vector2f &pos);
	~Rock();

	virtual void Pulse(float deltaTime) override;
	virtual void RenderGame(sf::RenderWindow *window) override;
	virtual void Event(sf::Event *e) override;

	void TakeHit() { m_hitsTaken++; }
	const auto& GetTakenHits() const { return m_hitsTaken; }
};

