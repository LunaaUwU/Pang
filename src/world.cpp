#include <SFML/Graphics.hpp>
#include "world.h"


void World::init() // Get all the sprites on start to avoid loading them every frame
{
    m_ground.setSize(sf::Vector2f(1920.f, 200.f));
    m_groundTexture.loadFromFile("../sprites/World/ground.png");
    m_ground.setTexture(&m_groundTexture);
    m_ground.setPosition(sf::Vector2f(0.f, 880.f));

    m_ceiling.setSize(sf::Vector2f(1920.f, 96.f));
    m_ceilingTexture.loadFromFile("../sprites/World/ceiling.png");
    m_ceiling.setTexture(&m_ceilingTexture);
    m_ceiling.setPosition(sf::Vector2f(0.f, 0.f));

    m_leftWall.setSize(sf::Vector2f(136.f, 784.f));
    m_leftWallTexture.loadFromFile("../sprites/World/left_wall.png");
    m_leftWall.setTexture(&m_leftWallTexture);
    m_leftWall.setPosition(sf::Vector2f(0.f, 96.f));

    m_rightWall.setSize(sf::Vector2f(136.f, 784.f));
    m_rightWallTexture.loadFromFile("../sprites/World/right_wall.png");
    m_rightWall.setTexture(&m_rightWallTexture);
    m_rightWall.setPosition(sf::Vector2f(1784.f, 96.f));

    m_backGroundTexture.loadFromFile("../sprites/World/back_ground.png");
    m_backGroundSprite.setTexture(m_backGroundTexture);
    m_backGroundSprite.setPosition(sf::Vector2f(136.f, 96.f));
}

// we divide render into background and walls, to be able to render the background first, then the walls
void World::renderWalls(sf::RenderWindow& window) const
{
    window.draw(m_ground);
    window.draw(m_ceiling);
    window.draw(m_leftWall);
    window.draw(m_rightWall);
}

void World::renderBackGround(sf::RenderWindow& window) const
{
    window.draw(m_backGroundSprite);
}