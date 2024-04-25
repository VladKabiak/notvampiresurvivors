#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Constants.h"

class MainMenu {
public:
    explicit MainMenu(sf::RenderWindow &window);

    void run();

private:
    sf::RenderWindow &m_window;
    sf::Texture m_backgroundTexture;
    sf::Sprite m_backgroundSprite;
    sf::Font m_font;
    sf::Text m_playButtonText;
    sf::Text m_exitButtonText;

    void render();
    bool handleInput();
    void switchToGame();
    bool isPlayButtonClicked(const int &mouseX, const int &mouseY) const;
    void initializeBackground();
    void initializeText();

    bool isExitButtonClicked(const int &mouseX, const int &mouseY) const;
};

#endif // MAIN_MENU_H

