#include <SFML/Graphics.hpp>
#include "passive.h"

#include "pooler.h"

void Passive::update(const sf::Int32 deltaMS, Player* player)
{
	if(m_active)
	{
		// the passives will fall down until they hit the ground
		if(m_passive.getPosition().y < 816)
		{
			m_passive.move(0.f, deltaMS * 0.5f);
		}

		// player collision
		if (m_passive.getPosition().y + 64 > 752 && m_passive.getPosition().x + 64 > player->getPlayerPos().x && m_passive.getPosition().x < player->getPlayerPos().x + 128)
		{
			activatePassiveAbility(player);
		}
	}

	// we need to use update to check if the frozen passive timer has run out
	if(startTimer)
	{
		if(clockTimer.getElapsedTime().asMilliseconds() >= 2000)
		{
			Ball::isFrozen = false;
			startTimer = false;
		}
	}
}

void Passive::render(sf::RenderWindow& window) const
{
	window.draw(m_passive);
}

void Passive::activate(float posX, float posY, int passiveType)
{
	m_active = true;
	m_passive.setPosition(sf::Vector2f(posX, posY));
	m_passiveType = passiveType;

	m_passive.setSize(sf::Vector2f(64, 64));
	m_passiveTexture.loadFromFile(m_passiveTypes.fileNames[m_passiveType]);
	m_passive.setTexture(&m_passiveTexture);
}

void Passive::deactivate()
{
	m_active = false;
	m_passive.setPosition(sf::Vector2f(-500, 0.f));
}

void Passive::activatePassiveAbility(Player* player)
{
	if(m_passiveType == 0)
	{
		player->multiplySpeed(1.25f);
	}
	else if(m_passiveType == 1)
	{
		if(startTimer)
		{
			clockTimer.restart();
		}
		startTimer = true;
		clockTimer.restart();
		Ball::isFrozen = true;
	}
	deactivate();
}

void Passive::restartGame()
{
	deactivate();
	startTimer = false;
}
