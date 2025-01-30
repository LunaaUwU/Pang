#pragma once
#include <SFML/Graphics.hpp>

class World
{
	public:

		void init();

		void renderWalls(sf::RenderWindow& window) const;

		void renderBackGround(sf::RenderWindow& window) const;

	private:
		sf::RectangleShape m_ground;
		sf::Texture m_groundTexture;

		sf::RectangleShape m_ceiling;
		sf::Texture m_ceilingTexture;

		sf::RectangleShape m_leftWall;
		sf::Texture m_leftWallTexture;

		sf::RectangleShape m_rightWall;
		sf::Texture m_rightWallTexture;

		sf::Sprite m_backGroundSprite;
		sf::Texture m_backGroundTexture;

};