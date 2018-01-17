#include "Object.h"

#include <Common\ParticleSystem.h>
#include <Common\Common.h>
#include <Common\ResourceMgr.h>

#include <SFML\Graphics.hpp>

Object::Object(const sf::Vector2f &pos)
{
	m_pSprite = std::make_shared<sf::Sprite>();
	m_pSprite->setPosition(pos);

	m_zDepth = pos.y;

	m_entity = false;

	m_pParent = nullptr;

	m_particleAnim = true;
	m_pParticles = nullptr;
}

Object::~Object()
{
}

void Object::Pulse(float deltaTime)
{
	if (m_pParticles)
	{
		m_pParticles->SetEmitterPos(GetPosition());
		m_pParticles->Pulse(deltaTime);

		if (m_particlesCreateTime.getElapsedTime().asMilliseconds() > m_particlesLifetime)
		{
			m_pParticles.reset();
		}
	}
}

void Object::RenderGame(sf::RenderWindow *window)
{
	if (m_pParticles)
		m_pParticles->Render(window);
}

void Object::CreateParticles(float dir)
{
	m_pParticles.reset(new ParticleSystem(100, sf::seconds(5)));
	m_pParticles->SetColor(m_particlesColor);
	m_pParticles->SetMaxSpeed(10.f);
	m_pParticles->SetMinSpeed(5.f);
	m_pParticles->SetSize(0.3f);
	m_pParticles->SetDirection(dir);
	m_pParticles->SetDirectionRange(40.f);

	m_particlesCreateTime.restart();
}

const sf::Vector2f& Object::GetPosition() const
{
	return m_pSprite->getPosition();
}

const sf::Vector2f& Object::GetOrigin() const
{
	return m_pSprite->getOrigin();
}

void Object::LoadTexture(const std::string &textureName, Directions::Direction dir)
{
	auto texture = ResourceMgr::Get()->GetTexture(textureName, dir);

	if (texture && texture->m_pTexture)
		m_textures[dir] = texture->m_pTexture.get();
}

void Object::LoadTextures(const std::string &textureName, TypeDirection typeDir)
{
	auto rsMgr = ResourceMgr::Get();

	if (!rsMgr)
	{
		printf("Resource manager isn't initialized!\n");
		return;
	}

	LoadTexture(textureName, Directions::NE);
	LoadTexture(textureName, Directions::NW);
	LoadTexture(textureName, Directions::SE);
	LoadTexture(textureName, Directions::SW);

	if (typeDir == FULL_DIRECTIONS)
	{
		LoadTexture(textureName, Directions::E);
		LoadTexture(textureName, Directions::N);
		LoadTexture(textureName, Directions::S);
		LoadTexture(textureName, Directions::W);
	}
}

void Object::SetTexture(Directions::Direction direction, bool center)
{
	if (m_textures.find(direction) != m_textures.end())
	{
		auto texture = m_textures[direction];

		m_pSprite->setTexture(*texture, true);

		if (center)
			m_pSprite->setOrigin(texture->getSize().x / 2.f, texture->getSize().y / 2.f);
	}
}
