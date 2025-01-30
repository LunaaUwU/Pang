#pragma once
#include <SFML/Graphics.hpp>
#include "harpoon.h"

class Player
{
public:
	void update(sf::Int32 deltaMS, sf::Clock startGameTimer);

	void render(sf::RenderWindow& window) const;

	void init();

	void move(sf::Int32 deltaMS, bool direction);

	void shoot();

	sf::Vector2f getHarpoonPos() const { return harpoon->getHarpoon().getPosition(); }
	bool getHarpoonActive() const { return harpoon->m_active; }
	Harpoon* getHarpoon() const { return harpoon; }
	sf::Vector2f getPlayerPos() const { return m_player.getPosition(); }
	void multiplySpeed(float newSpeed) { m_playerSpeed *= newSpeed; }

	void restartGame();


private:

	float m_playerSpeed = 0.5f;

	sf::RectangleShape m_player;
	sf::Texture m_playerMoveRTexture;

	sf::Texture m_playerMoveLTexture;

	sf::Texture m_playerShootRTexture;
	sf::Texture m_playerShootLTexture;

	bool m_spriteDirection = true; // true means right, false means left

	bool m_shooting = false;
	sf::Clock m_shootTimer;

	Harpoon* harpoon = new Harpoon();
};