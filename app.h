#pragma once
#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "Game.h"

enum class GameState { Menu, Playing, Exiting, GameOver };

class App
{
public:
    App();
    void run();

private:
    void processEvents();
    void update();
    void render();

private:
    sf::RenderWindow m_window;
    GameState m_state = GameState::Menu;

    Menu m_menu;
    Game m_game;
    sf::Font m_font;
    sf::Text m_gameOverText;
};
