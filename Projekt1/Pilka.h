#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Paletka.h"

class Pilka {
private:
    float x, y;
    float vx, vy;
    float radius;
    sf::CircleShape shape;

public:
    Pilka(float x, float y, float vx, float vy, float radius)
        : x(x), y(y), vx(vx), vy(vy), radius(radius)
    {
        shape.setRadius(radius);
        shape.setOrigin(radius, radius);
        shape.setFillColor(sf::Color::White);
        shape.setPosition(x, y);
    }

    void move() {
        x += vx;
        y += vy;
        shape.setPosition(x, y);
    }

    void bounceWalls(float width, float height) {
        if (x - radius <= 0 || x + radius >= width) {
            vx = -vx;
        }
        if (y - radius <= 0) {
            vy = -vy;
        }
    }

    bool collideWithPaddle(Paletka& paletka) {
        float paddleLeft = paletka.getX();
        float paddleRight = paletka.getX() + paletka.getSzerokosc();
        float paddleTop = paletka.getY();
        float paddleBottom = paletka.getY() + paletka.getWysokosc();

        bool withinX = (x + radius >= paddleLeft) && (x - radius <= paddleRight);
        bool hittingTop = (y + radius >= paddleTop) && (y - radius <= paddleBottom);

        if (withinX && hittingTop && vy > 0) {
            vy = -vy;
            y = paddleTop - radius;
            shape.setPosition(x, y);
            return true;
        }
        return false;
    }

    void draw(sf::RenderTarget& target) {
        target.draw(shape);
    }

    sf::FloatRect getGlobalBounds() const { return shape.getGlobalBounds(); }

    // Gettery i Settery
    float getVy() const { return vy; }
    void setVy(float n) { vy = n; }

    float getVx() const { return vx; }   
    void setVx(float n) { vx = n; }      

    
    sf::Vector2f getPosition() const { return sf::Vector2f(x, y); }
    sf::Vector2f getVelocity() const { return sf::Vector2f(vx, vy); }

    float getX() const { return x; }
    float getY() const { return y; }
    float getRadius() const { return radius; }

};

