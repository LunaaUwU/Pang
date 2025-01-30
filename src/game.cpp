#include "game.h"


void Game::update(const sf::Int32 deltaMS)
{
    // split update behaviour depending on the open menu
    if(isMainMenuActive)
    {
	    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	    {
            mainMenuSelection = false;
	    }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            mainMenuSelection = true;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && QuitGameTimer.getElapsedTime().asMilliseconds() >= 250)
        {
            if (mainMenuSelection)
            {
                startGameTimer.restart();
                isMainMenuActive = false;
                isDifficultyMenuActive = true;
            }
            else
            {
                closeGame = true; // we make this variable so we don't have to use the window inside the update function
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && QuitGameTimer.getElapsedTime().asMilliseconds() >= 250)
        {
            closeGame = true;
        }
    }
    else if(isDifficultyMenuActive)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            difficultySelection = false;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            difficultySelection = true;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && startGameTimer.getElapsedTime().asMilliseconds() >= 250)
        {
            if (difficultySelection)
            {
                startGameTimer.restart();
                ballsToSpawn = 1;
                passivePooler.setUsablePassives(2);
                isDifficultyMenuActive = false;
            }
            else
            {
                startGameTimer.restart();
                ballsToSpawn = 2;
                passivePooler.setUsablePassives(5);
                difficultySelection = true;
                isDifficultyMenuActive = false;
            }
            BallPooler::totalBalls = ballsToSpawn * 15;

            // spawn the first balls
            for(int i = 0; i < ballsToSpawn; i++)
            {
                ballPooler.activate(firstBallXSpeed, 0.5f, 832, 80, 0);
                firstBallXSpeed *= -1;
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && QuitGameTimer.getElapsedTime().asMilliseconds() >= 250)
        {
            difficultySelection = true;
            isDifficultyMenuActive = false;
            isMainMenuActive = true;
            QuitGameTimer.restart();
        }
    }
    else if (isPauseActive)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            pauseMenuSelection = false;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            pauseMenuSelection = true;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            if (pauseMenuSelection)
            {
                startGameTimer.restart();
                QuitGameTimer.restart();
                isPauseActive = false;
            }
            else
            {
                pauseMenuSelection = true;
                isPauseActive = false;
                isMainMenuActive = true;
                QuitGameTimer.restart();
                restart();
            }
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && QuitGameTimer.getElapsedTime().asMilliseconds() >= 250)
        {
            pauseMenuSelection = true;
            isPauseActive = false;
            isMainMenuActive = true;
            QuitGameTimer.restart();
            restart();
        }
    }
    else
    {
        // in-game update behaviour
        if (Ball::gameOver)
        {
            Ball::gameOver = false;
            gameOver = true;
            doOnce = true;
        }
        if (BallPooler::winGame)
        {
            BallPooler::winGame = false;
            winGame = true;
            doOnce = true;
        }

        if (!gameOver && !winGame)
        {
            // in-game update behaviour IF you have not won/lost let

            // player update also manages the harpoon update
            m_player->update(deltaMS, startGameTimer);
            // split the ballPooler update behaviour depending on the harpoon being inactive/not instantiated or activated
            // ballPooler update manages all the ball updates
            if (m_player->getHarpoon() != nullptr)
            {
                ballPooler.update(deltaMS, m_player->getHarpoonPos(), m_player->getHarpoonActive(), m_player->getPlayerPos(), ballPooler, m_player->getHarpoon(), passivePooler);
            }
            else
            {
                ballPooler.update(deltaMS, sf::Vector2f(0.f, 0.f), false, m_player->getPlayerPos(), ballPooler, m_player->getHarpoon(), passivePooler);
            }
            // passivePooler update manages all the passive updates
            passivePooler.update(deltaMS, m_player);
            // pause menu
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && QuitGameTimer.getElapsedTime().asMilliseconds() >= 250)
            {
                QuitGameTimer.restart();
                isPauseActive = true;
            }
        }
        else
        {
            // doOnce is used to restart the gameOverTimer only once
            if (doOnce)
            {
                gameOverTimer.restart();
                doOnce = false;
            }
        }
    }
}

void Game::render(sf::RenderWindow& window)
{
    // save the MS of the timer in a variable to save space later
    if (gameOver || winGame)
    {
        gameOverTimerAsMS = gameOverTimer.getElapsedTime().asMilliseconds();
    }
    window.clear();

    if(isMainMenuActive)
    {
	    if(mainMenuSelection)
	    {
            window.draw(m_mainMenuPlaySprite);
	    }
        else
        {
            window.draw(m_mainMenuQuitSprite);
        }
    }
    else if (isDifficultyMenuActive)
    {
        if (difficultySelection)
        {
            window.draw(m_difficultyMediumSprite);
        }
        else
        {
            window.draw(m_difficultyHardSprite);
        }
    }
    else
    {
        // in-game rendering

        // render the background first because it's the furthest back
        m_world->renderBackGround(window);

        // ballPooler render also manages all the balls rendering
        ballPooler.render(window);

        // passivePooler render also manages all the passives rendering
        passivePooler.render(window);

        // if we lost, we make the player flash by only being rendered in intervals
        if (gameOver)
        {
            if ((gameOverTimerAsMS >= 100 && gameOverTimerAsMS <= 400) || (gameOverTimerAsMS >= 500 && gameOverTimerAsMS <= 800) ||
                (gameOverTimerAsMS >= 900 && gameOverTimerAsMS <= 1200) || (gameOverTimerAsMS >= 1300 && gameOverTimerAsMS <= 1600) || (gameOverTimerAsMS >= 1700))
            {
                m_player->render(window);
            }
        }
        else
        {
            m_player->render(window);
        }

        m_world->renderWalls(window);

        // pause menu rendering
        if (isPauseActive)
        {
            if (pauseMenuSelection)
            {
                window.draw(m_pauseResumeSprite);
            }
            else
            {
                window.draw(m_pauseQuitSprite);
            }
        }

        // game over rendering
        if (gameOver)
        {
            window.draw(m_gameOverSprite);
            if (gameOverTimer.getElapsedTime().asMilliseconds() >= 2000)
            {
                gameOver = false;
                restart();
            }
        }

        // win rendering
        else if (winGame)
        {
            window.draw(m_winGameSprite);
            if (gameOverTimer.getElapsedTime().asMilliseconds() >= 2000)
            {
                winGame = false;
                restart();
            }
        }
    }

    // we manage closing the game in the render method so update doesn't need to get window as an argument
    if(closeGame)
    {
        window.close();
    }
    window.display();
}

void Game::init()
{
    // instantiates x balls * 15, since 1 big ball will split into 14 more balls. 2 is the biggest number with the current difficulties
    ballPooler.instantiate(2);
    // it will spawn a total of x passives, they can repeat and are random. 5 is the biggest number with the current difficulties
    passivePooler.instantiate(5);

    m_world->init();

    // player init also manages harpoon init
    m_player->init();

    // load every menu as well as the win/game over sprites
    m_gameOverTexture.loadFromFile("../sprites/game_over.png");
    m_gameOverSprite.setTexture(m_gameOverTexture);
    m_gameOverSprite.setPosition(sf::Vector2f(0.f, 412.f));

    m_winGameTexture.loadFromFile("../sprites/win_game.png");
    m_winGameSprite.setTexture(m_winGameTexture);
    m_winGameSprite.setPosition(sf::Vector2f(0.f, 412.f));

    m_mainMenuPlayTexture.loadFromFile("../sprites/Menu/main_menu_play.png");
    m_mainMenuPlaySprite.setTexture(m_mainMenuPlayTexture);
    m_mainMenuPlaySprite.setPosition(sf::Vector2f(0.f, 0.f));

    m_mainMenuQuitTexture.loadFromFile("../sprites/Menu/main_menu_quit.png");
    m_mainMenuQuitSprite.setTexture(m_mainMenuQuitTexture);
    m_mainMenuQuitSprite.setPosition(sf::Vector2f(0.f, 0.f));

    m_pauseResumeTexture.loadFromFile("../sprites/Menu/pause_resume.png");
    m_pauseResumeSprite.setTexture(m_pauseResumeTexture);
    m_pauseResumeSprite.setPosition(sf::Vector2f(0.f, 284.f));

    m_pauseQuitTexture.loadFromFile("../sprites/Menu/pause_quit.png");
    m_pauseQuitSprite.setTexture(m_pauseQuitTexture);
    m_pauseQuitSprite.setPosition(sf::Vector2f(0.f, 284.f));

    m_difficultyMediumTexture.loadFromFile("../sprites/Menu/difficulty_select_medium.png");
    m_difficultyMediumSprite.setTexture(m_difficultyMediumTexture);
    m_difficultyMediumSprite.setPosition(sf::Vector2f(0.f, 0.f));

    m_difficultyHardTexture.loadFromFile("../sprites/Menu/difficulty_select_hard.png");
    m_difficultyHardSprite.setTexture(m_difficultyHardTexture);
    m_difficultyHardSprite.setPosition(sf::Vector2f(0.f, 0.f));
}

void Game::restart()
{
    firstBallXSpeed = 0.5f;
    isMainMenuActive = true;
    m_player->restartGame();

    ballPooler.restartGame();

    passivePooler.restartGame();
}
