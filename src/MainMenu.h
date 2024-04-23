#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <SFML/Graphics.hpp>
#include "Game.h"

class MainMenu {
public:
    MainMenu(sf::RenderWindow &window);

    void run();

private:
    sf::RenderWindow m_window;
    sf::Texture m_backgroundTexture;
    sf::Sprite m_backgroundSprite;
    sf::Font m_font;
    sf::Text m_playButtonText;

    void render();
    bool handleInput();

    void switchToGame();
};

#endif // MAIN_MENU_H

