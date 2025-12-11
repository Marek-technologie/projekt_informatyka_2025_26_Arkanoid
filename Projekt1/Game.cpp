#include "Game.h"
#include <iostream>
#include <algorithm> 

Game::Game()
    : m_paletka(800.f / 2 - 50, 600.f - 30, 100, 20, 8.f),
    m_pilka(800.f / 2, 600.f / 2, 4.f, 4.f, 10.f)
{
    loadLevel();
}

void Game::reset()
{
    m_paletka.setPosition(800.f / 2 - 50, 600.f - 30);
    // Reset pilki 
    m_pilka = Pilka(800.f / 2, 600.f / 2, 4.f, -4.f, 10.f);
    m_bloki.clear();
    loadLevel();
    m_gameOver = false;
}

void Game::update(sf::Time dt) {
    if (m_gameOver) return;

    m_pilka.move();
    m_pilka.bounceWalls(800.f, 600.f);
    m_pilka.collideWithPaddle(m_paletka); 

    for (auto& brick : m_bloki)
    {
        if (brick.czyZniszczony()) continue;

        sf::FloatRect ballBounds = m_pilka.getGlobalBounds();
        sf::FloatRect brickBounds = brick.getGlobalBounds();
        sf::FloatRect intersection;

        // kolizja cegly
        if (ballBounds.intersects(brickBounds, intersection))
        {
            brick.trafienie();

            
            std::cout << "HIT" << std::endl;

            if (brick.czyZniszczony()) {
                std::cout << "DESTROYED" << std::endl;
            }

            // Fizyka odbicia
            if (intersection.width < intersection.height) {
                m_pilka.setVx(-m_pilka.getVx());
            }
            else {
                m_pilka.setVy(-m_pilka.getVy());
            }
        }
    }

    // Usuwanie
    m_bloki.erase(
        std::remove_if(
            m_bloki.begin(), m_bloki.end(),
            [](const Brick& b) { return b.czyZniszczony(); }
        ),
        m_bloki.end()
    );

    // Przegrana
    if (m_pilka.getY() - m_pilka.getRadius() > 600.f) {
        if (!m_gameOver) {
            std::cout << "LOOSE" << std::endl;
            m_gameOver = true;
        }
    }
}

void Game::draw(sf::RenderTarget& target)
{
    for (auto& b : m_bloki)
        if (!b.czyZniszczony())
            target.draw(b);

    m_paletka.draw(target);
    m_pilka.draw(target);
}

void Game::loadLevel()
{
    int kolumny = 10;
    int wiersze = 5;
    float szer = 70.f;
    float wys = 25.f;

    for (int y = 0; y < wiersze; y++)
    {
        for (int x = 0; x < kolumny; x++)
        {
            m_bloki.emplace_back(
                sf::Vector2f(20 + x * (szer + 5), 40 + y * (wys + 5)),
                sf::Vector2f(szer, wys),
                3
            );
        }
    }
}

// Save/Load 

void Game::saveGame()
{
    
    GameSnapshot snapshot;
    snapshot.capture(m_paletka, m_pilka, m_bloki);

    if (snapshot.saveToFile("savegame.txt")) {
        std::cout << "Zapisano gre!" << std::endl;
    }
    else {
        std::cout << "Blad zapisu!" << std::endl;
    }
}

bool Game::loadGame()
{
    GameSnapshot snapshot;
    if (snapshot.loadFromFile("savegame.txt")) {
        snapshot.apply(m_paletka, m_pilka, m_bloki);
        m_gameOver = false;
        std::cout << "Wczytano gre!" << std::endl;
        return true;
    }
    std::cout << "Nie udalo sie wczytac gry!" << std::endl;
    return false;

}
