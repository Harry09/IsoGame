#include "FinalBoss.h"

#include <Common\Common.h>

#include <Play\PlayScene.h>
#include <Play\Rooms\Room.h>
#include <Play\Objects\Entities\Player.h>
#include <Play\Objects\Entities\Bullet.h>

FinalBoss::FinalBoss(const sf::Vector2f &pos)
	: Boss(pos)
{
	LoadTextures("enemies\\spaceCraft4", TypeDirection::FULL_DIRECTIONS);
	SetTexture(Directions::E);

	m_health = m_maxHealth = 400;
	m_hitbox = sf::FloatRect(40, 32, 126, 112);

	m_dir = 0;

	m_scoreValue = 15000;                                                                                   

	m_stage = 1;

	m_moving = false;
}

FinalBoss::~FinalBoss()
{
}

void FinalBoss::Pulse(float deltaTime)
{
	switch (m_stage)
	{
		case 2:
		{
			if (m_lastShot.getElapsedTime().asMilliseconds() > 3000)
			{
				
				float dir = pointDir(GetPosition(), Player::Get()->GetPosition());

				Shot(dir, true);

				for (int i = 0; i < 5; ++i)
				{
					Shot(dir + 5 * i);
					Shot(dir - 5 * i);
				}

				if (float(m_health) < m_maxHealth * 0.40f)
					m_stage = 3;

				m_lastShot.restart();
			}
		} break;
		case 3:
		{
			if (m_lastShot.getElapsedTime().asMilliseconds() > 100)
			{
				Shot(m_dir + 180.f);
			}
		}
		case 1:
		{
			if (m_lastShot.getElapsedTime().asMilliseconds() > 100)
			{
				m_dir += 7;

				if (m_dir > 360.f)
					m_dir -= 360.f;

				Shot(m_dir, true);

				m_lastShot.restart();

				if (m_stage == 1 && float(m_health) < m_maxHealth * 0.75f)
					m_stage = 2;
			}
		} break;
	}
	
	Boss::Pulse(deltaTime);
}

void FinalBoss::Shot(float degree, bool changeTex)
{
	if (changeTex)
	{
		Directions::Direction dir;

		if (degree >= 0 && degree < 22.5f)
			dir = Directions::E;
		else if (degree >= 22.5f && degree < 67.5f)
			dir = Directions::NE;
		else if (degree >= 67.5f && degree < 112.5f)
			dir = Directions::N;
		else if (degree >= 112.5f && degree < 157.5f)
			dir = Directions::NW;
		else if (degree >= 157.5f && degree < 202.5f)
			dir = Directions::W;
		else if (degree >= 202.5f && degree < 247.5f)
			dir = Directions::SW;
		else if (degree >= 247.5f && degree < 292.5f)
			dir = Directions::S;
		else if (degree >= 292.5f && degree < 337.5f)
			dir = Directions::SE;
		else
			dir = Directions::E;

		SetTexture(dir);
	}

	m_pParent->AddObject(new Bullet(Bullet::UNFRIENDLY, GetPosition(), lenDir(4.f, degree)));
}

void FinalBoss::DeathEvent()
{
	PlayScene::Get()->EndGame(true);

	Enemy::DeathEvent();
}
