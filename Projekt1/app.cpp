#include "App.h"
#include <iostream>

App::App()
    : m_window(sf::VideoMode(800, 600), "Arkanoid"),
    m_menu(800, 600),
    m_game()
{
    m_window.setFramerateLimit(60);

    
    if (!m_font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Blad ladowania czcionki!" << std::endl;
    }

    m_gameOverText.setFont(m_font);
    m_gameOverText.setString("KONIEC GRY\n\n[R] - Restart\n[ESC] - Menu");
    m_gameOverText.setCharacterSize(30);
    m_gameOverText.setFillColor(sf::Color::Red);
    m_gameOverText.setOutlineColor(sf::Color::White);
    m_gameOverText.setOutlineThickness(1);

    // Wysrodkowanie tekstu
    sf::FloatRect textRect = m_gameOverText.getLocalBounds();
    m_gameOverText.setOrigin(textRect.left + textRect.width / 2.0f,
        textRect.top + textRect.height / 2.0f);
    m_gameOverText.setPosition(800 / 2.0f, 600 / 2.0f);
}

void App::run()
{
    while (m_window.isOpen())
    {
        processEvents();
        update();
        render();
    }
}

void App::processEvents()
{
    sf::Event e;
    while (m_window.pollEvent(e))
    {
        if (e.type == sf::Event::Closed)
            m_window.close();

        // 1. Obsluga MENU
        if (m_state == GameState::Menu)
        {
            if (e.type == sf::Event::KeyPressed)
            {
                if (e.key.code == sf::Keyboard::Up)
                    m_menu.moveUp();
                else if (e.key.code == sf::Keyboard::Down)
                    m_menu.moveDown();
                else if (e.key.code == sf::Keyboard::Enter)
                {
                    int selected = m_menu.getSelectedItem();
                    if (selected == 0) {
                        m_game.reset();
                        m_state = GameState::Playing;
                    }
                    else if (selected == 1) { 
                        if (m_game.loadGame()) m_state = GameState::Playing;
                    }
                    else if (selected == 2) {
                        m_state = GameState::Exiting;
                    }
                }
            }
        }
        // 2. Obsluga GRY
        else if (m_state == GameState::Playing)
        {
            if (e.type == sf::Event::KeyPressed)
            {
                if (e.key.code == sf::Keyboard::Escape)
                    m_state = GameState::Menu;
                else if (e.key.code == sf::Keyboard::R)
                    m_game.reset();
                else if (e.key.code == sf::Keyboard::F5)
                    m_game.saveGame();
            }
        }
        // 3. Obsluga GAME OVER (NOWE)
        else if (m_state == GameState::GameOver)
        {
            if (e.type == sf::Event::KeyPressed)
            {
                if (e.key.code == sf::Keyboard::R) // Restart
                {
                    m_game.reset();
                    m_state = GameState::Playing;
                }
                else if (e.key.code == sf::Keyboard::Escape) // Powrót do menu
                {
                    m_state = GameState::Menu;
                }
            }
        }
    }
}

void App::update()
{
    if (m_state == GameState::Playing)
    {
        // sterowanie paletki
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            m_game.movePaddleLeft(800.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            m_game.movePaddleRight(800.f);

        m_game.update(sf::seconds(1.f / 60.f));

        // sprawdzenie czy przegralismy
        if (m_game.isGameOver())
        {
            m_state = GameState::GameOver;
        }
    }
    else if (m_state == GameState::Exiting)
    {
        m_window.close();
    }
}

void App::render()
{
    m_window.clear(sf::Color(25, 30, 40));

    if (m_state == GameState::Menu)
    {
        m_menu.draw(m_window);
    }
    else if (m_state == GameState::Playing)
    {
        m_game.draw(m_window);
    }
    else if (m_state == GameState::GameOver) 
    {
        
        m_game.draw(m_window);

      
        sf::RectangleShape overlay(sf::Vector2f(800, 600));
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        m_window.draw(overlay);

       
        m_window.draw(m_gameOverText);
    }

    m_window.display();

}
