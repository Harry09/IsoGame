#include "ParticleSystem.h"

#include <SFML\Graphics.hpp>

ParticleSystem::ParticleSystem(unsigned int count, const sf::Time &lifetime)
	: m_particles(count), m_vertices(sf::Points, count), m_lifetime(lifetime), m_size(1.f)
{
	for (unsigned int i = 0; i < count; ++i)
	{
		m_particles[i].vertex = &m_vertices[i];
	}

	m_dir = -1.f;
	m_dirRange = -1.f;
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::Pulse(float deltaTime)
{
	for (auto& p : m_particles)
	{
		if (p.createTime.getElapsedTime() > p.lifetime)
			ResetParticle(p);

		p.velocity += m_gravity * deltaTime;
		p.vertex->position += p.velocity * deltaTime;

		float ratio = 1.f - float(p.createTime.getElapsedTime().asMilliseconds()) / p.lifetime.asMilliseconds();
		p.vertex->color.a = char(ratio * 255);
	}
}

void ParticleSystem::Render(sf::RenderWindow *window)
{
	auto state = sf::RenderStates::Default;

	state.transform.scale(sf::Vector2f(m_size, m_size));
	state.texture = nullptr;

	window->draw(m_vertices, state);
}

void ParticleSystem::ResetParticle(Particle &particle)
{
	float angle = 0;

	if (m_dir >= 0.f && m_dirRange >= 0.f)
		angle = m_random.Get<float>(m_dir - m_dirRange / 2.f, m_dir + m_dirRange / 2.f) * 3.14f / 100.f;
	else
		angle = m_random.Get<float>(0.f, 360.f) * 3.14f / 180.f;


	float speed = m_random.Get<float>(m_minSpeed, m_maxSpeed);
	particle.velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
	particle.createTime.restart();
	particle.lifetime = sf::milliseconds(m_random.Get<int>(m_lifetime.asMilliseconds() - 500, m_lifetime.asMilliseconds() + 500));
	particle.vertex->position = m_emitterPos / m_size;
	particle.vertex->color = m_color;
}
