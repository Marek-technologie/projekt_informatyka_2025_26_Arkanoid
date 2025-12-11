#include "Menu.h"
#include <iostream>

Menu::Menu(float width, float height)
{
    // Zamiast "fonts/arial.ttf" wpisz:
    if (!m_font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Blad ladowania czcionki!" << std::endl;
    }

    // ZMIANA: Mamy teraz 3 elementy
    m_items.resize(3);

    // 0: Nowa gra
    m_items[0].setFont(m_font);
    m_items[0].setString("Nowa gra");
    m_items[0].setPosition(width / 2 - 60, 200);

    // 1: Wczytaj gre (NOWE)
    m_items[1].setFont(m_font);
    m_items[1].setString("Wczytaj gre");
    m_items[1].setPosition(width / 2 - 60, 260);

    // 2: Wyjscie (Przesuniête w dó³)
    m_items[2].setFont(m_font);
    m_items[2].setString("Wyjscie");
    m_items[2].setPosition(width / 2 - 60, 320);
}

void Menu::draw(sf::RenderTarget& target)
{
    for (int i = 0; i < m_items.size(); i++)
    {
        m_items[i].setFillColor(i == m_selected ? sf::Color::Red : sf::Color::White);
        target.draw(m_items[i]);
    }
}

void Menu::moveUp()
{
    if (m_selected > 0)
        m_selected--;
}

void Menu::moveDown()
{
    if (m_selected < m_items.size() - 1)
        m_selected++;
}