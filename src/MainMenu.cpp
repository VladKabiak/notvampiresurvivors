#include "MainMenu.h"

MainMenu::MainMenu(sf::RenderWindow &window): m_window(window) {
    m_backgroundTexture.loadFromFile("../assets/textures/textures.png");
    m_backgroundSprite.setTexture(m_backgroundTexture);
    m_backgroundSprite.setTextureRect(sf::IntRect (640, 800, 384, 224));
    m_backgroundSprite.setScale(1280.0f / 384.0f, 720.0f / 224.0f);

    m_font.loadFromFile("../assets/fonts/Arial.ttf");

    m_playButtonText.setFont(m_font);
    m_playButtonText.setString("Play");
    m_playButtonText.setCharacterSize(30);
    m_playButtonText.setFillColor(sf::Color::White);

    sf::FloatRect textBounds = m_playButtonText.getLocalBounds();
    m_playButtonText.setOrigin(textBounds.left + textBounds.width / 2.0f,
                               textBounds.top + textBounds.height / 2.0f);
    m_playButtonText.setPosition(m_window.getSize().x / 2.0f, m_window.getSize().y / 2.0f);
}

void MainMenu::run() {
    while (m_window.isOpen()) {
        render();
        if (handleInput()) {
            Game game(m_window);
            game.run();
            return;
        }
    }
}

void MainMenu::render() {
    m_window.clear();
    m_window.draw(m_backgroundSprite);
    m_window.draw(m_playButtonText);
    m_window.display();
}

bool MainMenu::handleInput() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_window.close();
            return false;
        } else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                // Check if the play button is clicked
                sf::Vector2f mousePos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
                if (m_playButtonText.getGlobalBounds().contains(mousePos)) {
                    switchToGame();
                    return true; // Return true if play button is clicked
                }
            }
        }
    }
    return false;
}

void MainMenu::switchToGame() {
    Game game(m_window);
    game.run();
}
