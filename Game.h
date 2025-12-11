#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>   // DODANO: do zapisu plików
#include <iostream>  // DODANO: do wypisywania b³êdów
#include "Paletka.h"
#include "Pilka.h"
#include "Brick.h"

// Struktura pomocnicza
struct BlockData {
    float x, y;
    int punktyZycia;
};

// Klasa odpowiedzialna za zrzut stanu gry
class GameSnapshot {
private:
    sf::Vector2f paddlePosition;
    sf::Vector2f ballPosition;
    sf::Vector2f ballVelocity;
    std::vector<BlockData> blocks;

public:
    // Metoda pobieraj¹ca stan gry (Capture)
    void capture(const Paletka& paletka, const Pilka& pilka, const std::vector<Brick>& kamienie) {
        paddlePosition = paletka.getPaddlePosition();
        ballPosition = pilka.getPosition();
        ballVelocity = pilka.getVelocity();

        blocks.clear();
        blocks.reserve(kamienie.size());

        for (const auto& stone : kamienie) {
            // Zapisujemy tylko jeœli ceg³a nie jest zniszczona (lub zapisujemy wszystkie ¿ywe)
            if (!stone.czyZniszczony()) {
                BlockData data;
                data.x = stone.getPosition().x;
                data.y = stone.getPosition().y;
                data.punktyZycia = stone.getHP();
                blocks.push_back(data);
            }
        }
    }

    // Metoda aplikuj¹ca stan gry (Apply)
    void apply(Paletka& paletka, Pilka& pilka, std::vector<Brick>& kamienie) {
        // 1. Przywróæ paletkê
        paletka.setPosition(paddlePosition.x, paddlePosition.y);

        // 2. Przywróæ pi³kê (zak³adamy, ¿e Pilka ma publiczne pole shape lub settery - dodaliœmy settery wczeœniej)
        // Musimy u¿yæ ma³ego "hacka" lub setterów. W Pilka.h doda³em settery setVx/Vy, ale tutaj musimy ustawiæ te¿ pozycjê.
        // Najlepiej zresetowaæ pi³kê nowymi danymi.
        pilka = Pilka(ballPosition.x, ballPosition.y, ballVelocity.x, ballVelocity.y, pilka.getRadius());

        // 3. Przywróæ bloki
        kamienie.clear();
        sf::Vector2f rozmiar(70.f, 25.f); // Rozmiar ceg³y (taki sam jak w loadLevel)

        for (const auto& data : blocks) {
            kamienie.emplace_back(
                sf::Vector2f(data.x, data.y),
                rozmiar,
                data.punktyZycia
            );
        }
    }

    bool saveToFile(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) return false;

        file << "PADDLE " << paddlePosition.x << " " << paddlePosition.y << "\n";
        file << "BALL " << ballPosition.x << " " << ballPosition.y << " " << ballVelocity.x << " " << ballVelocity.y << "\n";

        file << "BLOCKS " << blocks.size() << "\n";
        for (const auto& b : blocks) {
            file << b.x << " " << b.y << " " << b.punktyZycia << "\n";
        }
        return true;
    }


    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;

        std::string label;
        if (!(file >> label >> paddlePosition.x >> paddlePosition.y)) return false;
        if (!(file >> label >> ballPosition.x >> ballPosition.y >> ballVelocity.x >> ballVelocity.y)) return false;

        int count;
        file >> label >> count;
        blocks.clear();
        for (int i = 0; i < count; i++) {
            BlockData b;
            file >> b.x >> b.y >> b.punktyZycia;
            blocks.push_back(b);
        }
        return true;
    }
};

class Game
{
public:
    Game();

    void update(sf::Time dt);
    void draw(sf::RenderTarget& target);
    void reset();

    // NOWE METODY
    void saveGame();
    bool loadGame();
    bool isGameOver() const { return m_gameOver; }
    bool m_gameOver = false;

    void movePaddleLeft(float windowWidth) {
        m_paletka.moveLeft();
        m_paletka.clampToBounds(windowWidth);
    }

    void movePaddleRight(float windowWidth) {
        m_paletka.moveRight();
        m_paletka.clampToBounds(windowWidth);
    }

private:
    Paletka m_paletka;
    Pilka m_pilka;
    std::vector<Brick> m_bloki;

    void loadLevel();
};