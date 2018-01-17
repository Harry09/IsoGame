#pragma once

#include "Object.h"

#include <SFML\Graphics\Color.hpp>

class Room;
class ParticleSystem;

class Portal : public Object
{
private:
	std::shared_ptr<sf::Sprite>			m_pClosedPotal;

	Directions::Direction				m_portalDir;
	
	int									m_alpha;
	bool								m_closingAnim;
	bool								m_openingAnim;

	Room								*m_pDest;

	sf::Color							m_openColor;

	std::shared_ptr<ParticleSystem>		m_particles;

	bool								m_isClosed;

public:
	static char *PURPLE_TEXTURE_NAME;
	static char *RED_TEXTURE_NAME;
	static char *GOLD_TEXTURE_NAME;
	static char *GRAY_TEXTURE_NAME;

public:
	Portal(const sf::Vector2f &pos, Directions::Direction dir);
	~Portal();

	void Pulse(float deltaTime) override;
	void RenderGame(sf::RenderWindow *window) override;
	void Event(sf::Event *e) override;

	auto& GetPortalDir() const { return m_portalDir; }

	void SetDest(Room *room);
	const auto GetDest() const { return m_pDest; }

	void Open(bool anim = true);
	void Close(bool anim = true);
};

