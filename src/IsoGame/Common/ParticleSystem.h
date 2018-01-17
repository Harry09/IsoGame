#pragma once

#include "Random.h"

#include <SFML\Graphics\Color.hpp>
#include <SFML\Graphics\VertexArray.hpp>
#include <SFML\System\Clock.hpp>
#include <SFML\System\Time.hpp>
#include <SFML\System\Vector2.hpp>

#include <vector>

namespace sf
{
	class RenderWindow;
}

class ParticleSystem
{
private:
	struct Particle
	{
		sf::Vector2f	velocity;
		sf::Clock		createTime;
		sf::Time		lifetime;
		sf::Vertex		*vertex;
	};

private:
	std::vector<Particle>	m_particles;
	sf::VertexArray			m_vertices;
	sf::Time				m_lifetime;

	sf::Color				m_color;

	float					m_minSpeed;
	float					m_maxSpeed;

	float					m_size;

	float					m_dir;
	float					m_dirRange;

	sf::Vector2f			m_emitterPos;
	sf::Vector2f			m_gravity;

	Random					m_random;

private:
	void ResetParticle(Particle &particle);

public:
	ParticleSystem(unsigned int count, const sf::Time &lifetime = sf::seconds(2.f));
	~ParticleSystem();

	void SetDirection(float dir)						{ m_dir = dir; }
	void SetDirectionRange(float range)					{ m_dirRange = range; }
	void SetEmitterPos(const sf::Vector2f &position)	{ m_emitterPos = position; }
	void SetGravity(const sf::Vector2f &gravity)		{ m_gravity = gravity; }
	void SetMinSpeed(float speed)						{ m_minSpeed = speed; }
	void SetMaxSpeed(float speed)						{ m_maxSpeed = speed; }
	// only rgb
	void SetColor(const sf::Color &color)				{ m_color = color; }
	void SetSize(float size)							{ m_size = size; }

	void Pulse(float deltaTime);
	void Render(sf::RenderWindow *window);
};
