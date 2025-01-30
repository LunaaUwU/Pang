#include <SFML/Graphics.hpp>
#include <harpoon.h>

void Harpoon::update(const sf::Int32 deltaMS)
{
    // if the harpoon has not yet reached the top, it will move up
    if(m_active && m_harpoon.getPosition().y >= 96)
    {
        m_harpoon.move(0, deltaMS / -1.5);
    }

    else if(!m_attached && m_active && m_harpoon.getPosition().y < 96)
    {
        m_attached = true;
        m_attachTimer.restart();
    }

    // timer to stop attaching forever
    if(m_attached)
    {
        if (m_attachTimer.getElapsedTime().asMilliseconds() >= 1000)
        {
            deactivate();
        }
    }
}

void Harpoon::render(sf::RenderWindow& window) const
{
    window.draw(m_harpoon);
}

void Harpoon::init() // Get all the sprites on start to avoid loading them every frame
{
    m_harpoon.setSize(sf::Vector2f(34.f, 800.f));
    m_harpoonTexture.loadFromFile("../sprites/harpoon.png");
    m_harpoon.setTexture(&m_harpoonTexture);
    m_harpoon.setPosition(sf::Vector2f(0.f, -2500.f));
    m_active = false;
}

void Harpoon::setSpawnPosition(float x, float y)
{
    m_harpoon.setPosition(sf::Vector2f(x, y));
    m_active = true;
}

void Harpoon::deactivate()
{
    m_active = false;
    m_harpoon.setPosition(0.f, -2500.f);
    m_attached = false;
}
