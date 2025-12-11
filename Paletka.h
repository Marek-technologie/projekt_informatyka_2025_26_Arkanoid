#pragma once
#include <SFML/Graphics.hpp>

class Paletka {
private:
    float x, y;
    float szerokosc, wysokosc;
    float predkosc;
    sf::RectangleShape shape;

public:
    Paletka(float x, float y, float szerokosc, float wysokosc, float predkosc)
        : x(x), y(y), szerokosc(szerokosc), wysokosc(wysokosc), predkosc(predkosc)
    {
        shape.setSize(sf::Vector2f(szerokosc, wysokosc));
        shape.setFillColor(sf::Color::White);
        shape.setPosition(x, y);
    }

    void setPosition(float nowyX, float nowyY) {
        x = nowyX;
        y = nowyY;
        shape.setPosition(x, y);
    }

    void moveLeft() {
        x -= predkosc;
        shape.setPosition(x, y);
    }

    void moveRight() {
        x += predkosc;
        shape.setPosition(x, y);
    }

    void clampToBounds(float width) {
        if (x < 0) x = 0;
        if (x + szerokosc > width) x = width - szerokosc;
        shape.setPosition(x, y);
    }

    void draw(sf::RenderTarget& target) {
        target.draw(shape);
    }

    // NAPRAWIONE: Zwracanie wektora
    sf::Vector2f getPaddlePosition() const { return sf::Vector2f(x, y); }

    float getX() const { return x; }
    float getY() const { return y; }
    float getSzerokosc() const { return szerokosc; }
    float getWysokosc() const { return wysokosc; }
};