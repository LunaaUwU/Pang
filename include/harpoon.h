#pragma once
#include <SFML/Graphics.hpp>

class Harpoon
{
public:
	void update(sf::Int32 deltaMS);

	void render(sf::RenderWindow& window) const;

	void init();

	void setSpawnPosition(float x, float y);

	void deactivate();

	sf::RectangleShape getHarpoon() const { return m_harpoon; }
	bool m_active = false;
private:

	sf::RectangleShape m_harpoon;
	sf::Texture m_harpoonTexture;
	sf::Clock m_attachTimer;
	bool m_attached = false;

};