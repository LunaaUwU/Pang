#pragma once
#include <SFML/Graphics.hpp>
#include "ball.h"
#include "passive.h"

class BallPooler
{
public:

	void instantiate(int numberToPool);

	void activate(float speedX, float speedY, float posX, float posY, int ballSize);

	void render(sf::RenderWindow& window) const;

	void update(sf::Int32 deltaMS, sf::Vector2f harpoonPos, bool harpoonActive, sf::Vector2f playerPos, BallPooler& pooler, Harpoon* harpoon, PassivePooler& passivePooler) const;

	static void addTotalBalls(int numberToAdd);

	float getBallRegularSpeed() const { return m_ballRegularSpeed; }

	static int getTotalBalls() { return totalBalls; }

	void restartGame();

	inline static int totalBalls = 0;

	inline static bool winGame = false;

private:

	float firstBallXSpeed = 0.5f;
	float m_ballRegularSpeed = 0.5f;
	int m_activatedBalls = 0;
	int m_allInstantiatedBalls = 0;
	std::vector<Ball*> m_ballArray;
};

class PassivePooler
{
public:

	void update(sf::Int32 deltaMS, Player* player) const;

	void render(sf::RenderWindow& window) const;

	void instantiate(int numberToPool);

	void activate(float posX, float posY, int passiveType);

	int getTotalPassives() const { return m_totalPassives; }

	int getPassivesLeft() const { return m_usablePassives - m_activatedPassives; }

	void restartGame();

	void setUsablePassives(int usablePassives);

private:

	int m_totalPassives = 0;
	int m_usablePassives = 0;
	std::vector<Passive*> m_passiveArray;

	int m_activatedPassives = 0;
};