#include <SFML/Graphics.hpp>
#include <vector>
#include "pooler.h"
#include "game.h"

// instantiates deactivated balls
void BallPooler::instantiate(int numberToPool)
{
	// 1 pool is 1 big ball, which is 15 total balls
	for(int i = 0; i < numberToPool * 15; i++)
	{
		m_ballArray.push_back(new Ball());
	}
	m_allInstantiatedBalls = numberToPool * 15;
}

// when activating previously activated balls, I ran into a problem where if you change the texture of a sprite that already had a texture, it would bug out
// so now, the pooler will re-use the balls. How does it do it? Well, first it will start looking for previously activated balls with the size you want
// for example: if you play a game and hit the first ball, it will spawn 2 balls with size 1. If you restart, it will first spawn the big ball you already spawned before
// because it already has the texture we want (size 0 texture), and the two balls with size 1, will be the same as in the same game, because they already have the texture with size 1
void BallPooler::activate(float speedX, float speedY, float posX, float posY, int ballSize)
{
	// when activating a ball, the pooler will first search if a ball of the size you are looking for has already been created. This will also work after you finish a game or restart
	std::cout << "Attempting to spawn a ball of size " << ballSize << "... ";
	int i = 0;

	// looking for balls with preferred size that are not active at that moment
	for (const Ball* ball : m_ballArray)
	{
		if (ball->getBallSize() == ballSize && !ball->getActive())
		{
			std::cout << "Inactive ball with preferred size found" << std::endl;
			m_ballArray[i]->activate(speedX, speedY, posX, posY, ballSize);
			return;
		}
		i++;
	}

	// if there are no balls with preferred size, it will activate one that has not yet activated
	std::cout << "Ball with preferred size not found... ";
	int j = 0;
	for (const Ball* ball : m_ballArray)
	{
		if (ball->getBallSize() == -1 && !ball->getActive())
		{
			std::cout << "Inactive ball found" << std::endl;
			m_ballArray[j]->activate(speedX, speedY, posX, posY, ballSize);
			return;
		}
		j++;
	}

	// if there are no balls left with the size wanted, and no balls left to operate with, we will spawn a new ball to avoid problems
	std::cout << "No ball found, creating new ball..." << std::endl;
	m_ballArray.push_back(new Ball());
	m_allInstantiatedBalls++;
	m_ballArray[m_allInstantiatedBalls - 1]->activate(speedX, speedY, posX, posY, ballSize);

}

void BallPooler::render(sf::RenderWindow& window) const
{
	for(const Ball* ball : m_ballArray)
	{
		ball->render(window);
	}
}


void BallPooler::update(const sf::Int32 deltaMS, sf::Vector2f harpoonPos, bool harpoonActive, sf::Vector2f playerPos, BallPooler& pooler, Harpoon* harpoon, PassivePooler& passivePooler) const
{
	for(Ball* ball : m_ballArray)
	{
		ball->update(deltaMS, harpoonPos, harpoonActive, playerPos, pooler, harpoon, passivePooler, Passive::getTotalPassiveTypes());
	}
}

// this method will have a count of all the current balls left in the game, to check if we have won
void BallPooler::addTotalBalls(int numberToAdd)
{
	totalBalls += numberToAdd;
	if (totalBalls <= 0)
	{
		winGame = true;
	}
}

void BallPooler::restartGame()
{
	m_activatedBalls = 0;
	for (Ball* ball : m_ballArray)
	{
		ball->restartGame();
	}
	firstBallXSpeed = 0.5f;
	if(Ball::isFrozen)
	{
		Ball::isFrozen = false;
	}
}




void PassivePooler::update(const sf::Int32 deltaMS, Player* player) const
{
	for (Passive* passive : m_passiveArray)
	{
		passive->update(deltaMS, player);
	}
}

void PassivePooler::render(sf::RenderWindow& window) const
{
	for(const Passive* passive : m_passiveArray)
	{
		passive->render(window);
	}
}

void PassivePooler::instantiate(int numberToPool)
{
	for (int i = 0; i < numberToPool; i++)
	{
		m_passiveArray.push_back(new Passive());
	}
	m_totalPassives = numberToPool;
}

// same behaviour as the ballPooler activate method, read the comments there if you don't understand something
void PassivePooler::activate(float posX, float posY, int passiveType)
{
	std::cout << "Attempting to spawn a passive of type " << passiveType << "... ";
	int i = 0;
	for (const Passive* passive : m_passiveArray)
	{
		if (passive->getPassiveType() == passiveType && !passive->getActive())
		{
			std::cout << "Inactive passive with preferred size found" << std::endl;
			m_passiveArray[i]->activate(posX, posY, passiveType);
			m_activatedPassives++;
			return;
		}
		i++;
	}

	std::cout << "Passive with preferred size not found... ";
	int j = 0;
	for (const Passive* passive : m_passiveArray)
	{
		if (passive->getPassiveType() == -1 && !passive->getActive())
		{
			std::cout << "Inactive passive found" << std::endl;
			m_passiveArray[j]->activate(posX, posY, passiveType);
			m_activatedPassives++;
			return;
		}
		j++;
	}

	std::cout << "No passive found, creating new passive..." << std::endl;
	m_passiveArray.push_back(new Passive());
	m_totalPassives++;
	m_passiveArray[m_totalPassives - 1]->activate(posX, posY, passiveType);

}

void PassivePooler::restartGame()
{
	for (Passive* passive : m_passiveArray)
	{
		passive->restartGame();
	}
	m_activatedPassives = 0;
}

// changes depending on the chosen difficulty
void PassivePooler::setUsablePassives(int usablePassives)
{
	m_usablePassives = usablePassives;
}
