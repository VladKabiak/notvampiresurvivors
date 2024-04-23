#include "MainMenu.h"

MainMenu::MainMenu(sf::RenderWindow &window):
        m_window(window),
        m_backgroundTexture(),
        m_backgroundSprite(),
        m_font(),
        m_playButtonText() {

    initializeBackground();
    initializeText();
}

void MainMenu::initializeBackground() {
    if (!m_backgroundTexture.loadFromFile(TEXTURES_PATH)) {
        // Можно как-то обрабатывать ошибку, если адрес не правильынй
    }
    m_backgroundSprite.setTexture(m_backgroundTexture);
    m_backgroundSprite.setTextureRect(sf::IntRect(640, 800, 384, 224));
    m_backgroundSprite.setScale(m_window.getSize().x / 384.0f, m_window.getSize().y / 224.0f);
}

void MainMenu::initializeText() {
    if (!m_font.loadFromFile(FONT_PATH)) {
        // Можно как-то обрабатывать ошибку, если адрес не правильынй
    }
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
            switchToGame();
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
    sf::Event event{};
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_window.close();
            return false;
        } else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                if (isPlayButtonClicked(event.mouseButton.x, event.mouseButton.y)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool MainMenu::isPlayButtonClicked(const int &mouseX, const int &mouseY) const {
    sf::Vector2f mousePos = sf::Vector2f(mouseX, mouseY);
    return m_playButtonText.getGlobalBounds().contains(mousePos);
}

void MainMenu::switchToGame() {
    Game game(m_window);
    game.run();
}
