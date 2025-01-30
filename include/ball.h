#pragma once
#include <SFML/Graphics.hpp>
#include "harpoon.h"


class BallPooler;
class PassivePooler;

// depending on the ball size, it will have different characteristics, and they're stored here for easy use
struct BallSizes
{
	const char* fileNames[4] = { "../sprites/Ball/big_ball.png", "../sprites/Ball/medium_big_ball.png",
		"../sprites/Ball/medium_ball.png", "../sprites/Ball/small_ball.png" };

	const float sizeX[4] = { 256, 128, 64, 32 };
	const float sizeY[4] = { 186, 89, 43, 26 };

	const float hitPointX[4] = { 1528, 1656, 1720, 1752 };
	const float hitPointY[4] = { 694, 791, 837, 854 };

	const float m_maxSpeedY[4] = {1.3f, 1.15f, 1.f, 0.75f};

	const float m_hitboxOffset[4] = { 15.f, 12.f, 9.f, 6.f };
};

class Ball
{
public:

	void update(sf::Int32 deltaMS, sf::Vector2f harpoonPos, bool harpoonActive, sf::Vector2f playerPos, BallPooler& ballPooler, Harpoon* harpoon, PassivePooler& passivePooler, int totalPassiveTypes);

	void render(sf::RenderWindow& window) const;

	void setPosition(float posX, int posY);

	void deactivate();

	void activate(float speedX, float speedY, float posX, float posY, int ballSize);

	void spawnTwo(BallPooler& pooler) const;

	void restartGame();

	int getBallSize() const { return m_ballSize; }

	bool getActive() const { return m_active; }

	inline static bool gameOver = false;

	inline static bool isFrozen = false;

protected:

	sf::RectangleShape m_ball;
	sf::Texture m_ballTexture;

	float m_speedX = 0.5f;
	float m_speedY = 0.f;
	float m_hitPointX = 0.f;
	float m_hitPointY = 0.f;

	int m_ballSize = -1; // starts at -1 to let the ballPooler know it has yet not activated either in the current game, or previous games after restarting

	bool m_active = false;

	BallSizes m_ballSizes;

	float m_ballWidth = 0;
	float m_ballHeight = 0;

	bool waitBool = false;
	sf::Clock waitClock;

	float m_accelerationY = 0.00144f;
	bool m_falling = true;	
};