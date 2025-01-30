#pragma once
#include <SFML/Graphics.hpp>
#include "world.h"
#include "player.h"
#include "ball.h"
#include "pooler.h"


class Game
{
public:

    void update(sf::Int32 deltaMS);

    void render(sf::RenderWindow& window);

    void init();

    void restart();

    inline static int ballsToSpawn = 0;

private:

    BallPooler ballPooler;
    PassivePooler passivePooler;

    World* m_world = new World();
    Player* m_player = new Player();

    float firstBallXSpeed = 0.5f;
    bool gameOver = false;
    bool doOnce = true;
    sf::Clock gameOverTimer;
    float gameOverTimerAsMS = 0;

    bool winGame = false;

    bool isMainMenuActive = true;
    bool mainMenuSelection = true; // true means up (play), false means down (quit)
    bool closeGame = false;

    bool isPauseActive = false;
    bool pauseMenuSelection = true; // true means up (resume), false means down (restart)

    bool isDifficultyMenuActive = false;
    bool difficultySelection = true; // true means left (medium), false means right (hard)

    sf::Sprite m_gameOverSprite;
    sf::Texture m_gameOverTexture;

    sf::Sprite m_winGameSprite;
    sf::Texture m_winGameTexture;

    sf::Clock startGameTimer; // these two clocks avoid pressing esc or space in two consecutive frames in the menus, for example, pausing mid-game with esc would also instantly quit
    sf::Clock QuitGameTimer;

    sf::Sprite m_mainMenuPlaySprite;
    sf::Texture m_mainMenuPlayTexture;
    sf::Sprite m_mainMenuQuitSprite;
    sf::Texture m_mainMenuQuitTexture;

    sf::Sprite m_pauseResumeSprite;
    sf::Texture m_pauseResumeTexture;
    sf::Sprite m_pauseQuitSprite;
    sf::Texture m_pauseQuitTexture;

    sf::Sprite m_difficultyMediumSprite;
    sf::Texture m_difficultyMediumTexture;
    sf::Sprite m_difficultyHardSprite;
    sf::Texture m_difficultyHardTexture;
};
