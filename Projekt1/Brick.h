#pragma once
#include <SFML/Graphics.hpp>
#include <array> 

class Brick : public sf::RectangleShape {

private:
    int punktyZycia;
    bool jestZniszczony;

public:
    // Konstruktor
    Brick(sf::Vector2f startPo, sf::Vector2f rozmiar, int L) {
        punktyZycia = L;
        jestZniszczony = false;

        this->setOutlineColor(sf::Color::Black); 
        this->setOutlineThickness(1.f);          
        this->setSize(rozmiar);
        this->setPosition(startPo);

        aktualizujKolor();
    }

    // Obsluga trafienia
    void trafienie() {
        if (jestZniszczony)
            return;

        punktyZycia--;
        aktualizujKolor();

        if (punktyZycia <= 0)
            jestZniszczony = true;
    }

    // Aktualizacja koloru w zaleznoœci od zycia
    void aktualizujKolor() {
        static const std::array<sf::Color, 4> colorLUT = {
            sf::Color::Transparent, // 0 HP 
            sf::Color::Yellow,      // 1 HP
            sf::Color::Magenta,     // 2 HP
            sf::Color::Red          // 3 HP
        };

        if (punktyZycia > 0 && punktyZycia < 4) {
            this->setFillColor(colorLUT[punktyZycia]);
        }
    }

    // Rysowanie
    void draw(sf::RenderTarget& window) {
        if (!jestZniszczony) {
            window.draw(*this);
        }
    }

    // Gettery
    bool czyZniszczony() const { return jestZniszczony; }
    int getHP() const { return punktyZycia; }

};

