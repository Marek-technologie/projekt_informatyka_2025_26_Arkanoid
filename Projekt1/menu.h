#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Menu
{
public:
    Menu(float width, float height);

    void draw(sf::RenderTarget& target);
    void moveUp();
    void moveDown();
    int getSelectedItem() const { return m_selected; }

private:
    int m_selected = 0;
    sf::Font m_font;
    std::vector<sf::Text> m_items;
};
