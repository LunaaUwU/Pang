#pragma once
#include <SFML/Graphics.hpp>
#include "player.h"

#include <iostream>

class BallPooler;

struct PassiveTypes
{
	const char* fileNames[2] = { "../sprites/Passives/speed_player.png", "../sprites/Passives/speed_ball.png" };
};

class Passive
{
public:
	void update(sf::Int32 deltaMS, Player* player);

	void render(sf::RenderWindow& window) const;

	void activate(float posX, float posY, int passiveType);

	void deactivate();

	void activatePassiveAbility(Player* player);

	static int getTotalPassiveTypes() { return totalPassiveTypes; }

	void restartGame();

	int getPassiveType() const { return m_passiveType; }

	bool getActive() const { return m_active; }

private:
		
	sf::RectangleShape m_passive;
	sf::Texture m_passiveTexture;

	PassiveTypes m_passiveTypes;

	int m_passiveType = -1;
	
	bool m_active = false;

	bool startTimer = false;
	sf::Clock clockTimer;

	constexpr inline static int totalPassiveTypes = sizeof(PassiveTypes::fileNames) / sizeof(char*);
};