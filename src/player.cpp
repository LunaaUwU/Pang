#include <SFML/Graphics.hpp>
#include "player.h"

void Player::update(const sf::Int32 deltaMS, sf::Clock startGameTimer)
{
    // while shooting, you will stay still and change your sprite
    if (m_shooting) 
    {
        if (m_shootTimer.getElapsedTime().asMilliseconds() >= 250) 
        {
            m_shooting = false;
            if (!m_spriteDirection)
            {
                m_player.setTexture(&m_playerMoveLTexture);
            }
            else
            {
                m_player.setTexture(&m_playerMoveRTexture);
            }
        }
    }
    else
    {
        // to avoid shooting the same frame you press start with space
        if(startGameTimer.getElapsedTime().asMilliseconds() >= 300)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !m_shooting)
            {
                shoot();
            }
        }
        
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) && !m_shooting)
        {
            move(deltaMS, false);
        }
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) && !m_shooting)
        {
            move(deltaMS, true);
        }

        // avoid moving more than the walls allow you to
        if (m_player.getPosition().x < 136.f)
        {
            m_player.setPosition(136.f, m_player.getPosition().y);
        }
        if (m_player.getPosition().x > 1656.f)
        {
            m_player.setPosition(1656.f, m_player.getPosition().y);
        }
    }

    if(harpoon != nullptr)
    {
        harpoon->update(deltaMS);
    }
    
}

void Player::render(sf::RenderWindow & window) const
{
    if (harpoon != nullptr)
    {
        window.draw(harpoon->getHarpoon());
    }

    window.draw(m_player);
}

void Player::init()
{
    m_player.setSize(sf::Vector2f(128.f, 128.f));
    m_playerMoveRTexture.loadFromFile("../sprites/Player/player_move_right.png");
    m_player.setTexture(&m_playerMoveRTexture);
    m_player.setPosition(sf::Vector2f(896.f, 752.f));
    
    m_playerMoveLTexture.loadFromFile("../sprites/Player/player_move_left.png");
    m_playerShootLTexture.loadFromFile("../sprites/Player/player_shoot_left.png");
    m_playerShootRTexture.loadFromFile("../sprites/Player/player_shoot_right.png");

    harpoon->init();
}

void Player::move(sf::Int32 deltaMS, bool direction)
{
    if (!direction)
    {
        m_player.setTexture(&m_playerMoveLTexture);
        m_spriteDirection = false;
        m_player.move(deltaMS * -m_playerSpeed, 0.f);
    }
    else
    {
        m_player.setTexture(&m_playerMoveRTexture);
        m_spriteDirection = true;
        m_player.move(deltaMS * m_playerSpeed, 0.f);
    }
    
}

void Player::shoot()
{
    if (!m_spriteDirection)
    {
        m_player.setTexture(&m_playerShootLTexture);
    }
    else
    {
        m_player.setTexture(&m_playerShootRTexture);
    }
    harpoon->setSpawnPosition(m_player.getPosition().x + 50.f, m_player.getPosition().y);
    m_shooting = true;
    m_shootTimer.restart(); // shoot timer will make you stay with the shooting animation and make you unable to move
}

void Player::restartGame()
{
    m_player.setPosition(896.f, 752.f);
    harpoon->deactivate();
    m_shooting = false;
    m_spriteDirection = true;
    m_player.setTexture(&m_playerMoveRTexture);
    m_playerSpeed = 0.5f;
}