#include <SFML/Graphics.hpp>
#include "ball.h"
#include "pooler.h"

void Ball::update(const sf::Int32 deltaMS, sf::Vector2f harpoonPos, bool harpoonActive, sf::Vector2f playerPos, BallPooler& ballPooler, Harpoon* harpoon, PassivePooler& passivePooler, int totalPassiveTypes)
{
    // only execute update if the ball is active
    if (m_active)
    {
        // if frozen, don't execute move update
        if(!isFrozen)
        {
            // collision with the right wall
            if (m_ball.getPosition().x > m_hitPointX)
            {
                m_speedX = -ballPooler.getBallRegularSpeed();
            }

            // collision with the left wall
            else if (m_ball.getPosition().x < 136.f)
            {
                m_speedX = ballPooler.getBallRegularSpeed();
            }

            // collision with the ceiling
            if (m_ball.getPosition().y < 96)
            {
                m_ball.setPosition(m_ball.getPosition().x, 96);
                m_falling = true;
                m_speedY = 0;
            }

            // collision with the ground
            else if (m_ball.getPosition().y > m_hitPointY)
            {
                m_falling = false;
                m_speedY = -m_ballSizes.m_maxSpeedY[m_ballSize];
            }

            // accelerate or decelerate
            if (m_falling)
            {
                m_speedY += m_accelerationY * deltaMS;
            }
            else
            {
                m_speedY -= m_accelerationY * deltaMS;
            }

            // it reaches the highest point, so it starts falling again
            if (m_speedY <= 0)
            {
                m_falling = true;
            }

            m_ball.move(m_speedX * deltaMS, m_speedY * deltaMS);
        }

        if (harpoonActive)
        {
            // harpoon collision
            if (m_ball.getPosition().y + m_ballHeight > harpoonPos.y && m_ball.getPosition().x + m_ballWidth > harpoonPos.x && m_ball.getPosition().x < harpoonPos.x + 34)
            {
                harpoon->deactivate();
                srand(time(0));
                // generates a random number between 0 and the total balls left in the game.
                // the total balls will decrease every time you hit a ball, so each time you hit a ball, the chances increase to make sure all instantiated passives spawn
                const int randomNumber = rand() % BallPooler::getTotalBalls();
                if (randomNumber < passivePooler.getPassivesLeft())
                {
                    passivePooler.activate(m_ball.getPosition().x, m_ball.getPosition().y, rand() % totalPassiveTypes);
                }
                if(m_ballSize < 3)
                {
                    // mini delay to spawn balls to not cause any issue, not noticeable in game
                    waitClock.restart();
                    waitBool = true;
                }
                else
                {
                    deactivate();
                }
                ballPooler.addTotalBalls(-1);
            }
        }

        // player collision
        if(m_ball.getPosition().y + m_ballHeight > 752 + m_ballSizes.m_hitboxOffset[m_ballSize] && m_ball.getPosition().x + m_ballWidth > playerPos.x + m_ballSizes.m_hitboxOffset[m_ballSize] && m_ball.getPosition().x < playerPos.x + 128 - m_ballSizes.m_hitboxOffset[m_ballSize])
        {
            gameOver = true;
        }

        // after the mini delay, it spawns two balls
        if(waitBool)
        {
	        if(waitClock.getElapsedTime().asMilliseconds() >= 20)
	        {
                spawnTwo(ballPooler);                
                waitBool = false;
                deactivate();
	        }
        }
    }
}

void Ball::render(sf::RenderWindow& window) const
{
    window.draw(m_ball);
}

void Ball::setPosition(float posX, int posY)
{
    m_ball.setPosition(posX, posY);
}

void Ball::deactivate()
{
    m_active = false;
    m_ball.setPosition(sf::Vector2f(-500, 0.f));
}

void Ball::activate(float speedX, float speedY, float posX, float posY, int ballSize)
{
    m_active = true;
    m_ball.setPosition(sf::Vector2f(posX, posY));
    m_speedX = speedX;
    m_speedY = speedY;

    m_ballSize = ballSize;
    m_ballWidth = m_ballSizes.sizeX[m_ballSize];
    m_ballHeight = m_ballSizes.sizeY[m_ballSize];

    m_ball.setSize(sf::Vector2f(m_ballWidth, m_ballHeight));
    m_ballTexture.loadFromFile(m_ballSizes.fileNames[m_ballSize]);
    m_ball.setTexture(&m_ballTexture);
    m_hitPointX = m_ballSizes.hitPointX[m_ballSize];
    m_hitPointY = m_ballSizes.hitPointY[m_ballSize];
}

void Ball::spawnTwo(BallPooler& pooler) const
{
    pooler.activate(-pooler.getBallRegularSpeed(), -0.1f, m_ball.getPosition().x, m_ball.getPosition().y, m_ballSize + 1);
    pooler.activate(pooler.getBallRegularSpeed(), -0.1f, m_ball.getPosition().x + m_ballWidth - m_ballSizes.sizeX[m_ballSize + 1], m_ball.getPosition().y, m_ballSize + 1);
}

void Ball::restartGame()
{
    deactivate();
    gameOver = false;
    waitBool = false;
}
