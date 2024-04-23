#include "experiencebar.h"

ExperienceBar::ExperienceBar(float width, float height):
    m_background(sf::Vector2f(width, height)),
    m_fill(sf::Vector2f(0, height)),
    m_maxWidth(width) {

    m_background.setFillColor(sf::Color::White);
    m_background.setOutlineColor(sf::Color::Black);
    m_background.setOutlineThickness(2.0f);

    m_fill.setFillColor(sf::Color::Blue);

    m_levelFont.loadFromFile(FONT_PATH);
    m_levelText.setFont(m_levelFont);
    m_levelText.setFillColor(sf::Color::White);
    m_levelText.setString("Lvl " + std::to_string(m_level));
}

void ExperienceBar::setPosition(float x, float y) {
    m_background.setPosition(x, y);
    m_fill.setPosition(x, y);
    m_levelText.setPosition(x + m_maxWidth + 10.f, y - 10.f);
}

float ExperienceBar::getProgress() {
    return progress;
}

void ExperienceBar::setProgress(float progr) {
    progress = progr;

    // Изменяем ширину заполнения пропорционально прогрессу
    float fillWidth = m_maxWidth * progress;
    m_fill.setSize(sf::Vector2f(fillWidth, m_background.getSize().y));
}

void ExperienceBar::setLevel() {
    m_levelText.setString("Lvl " + std::to_string(++m_level));
}


void ExperienceBar::setLevel(int level) {
    m_levelText.setString("Lvl " + std::to_string(level));
}


void ExperienceBar::render(sf::RenderWindow& window) {
    window.draw(m_background);
    window.draw(m_fill);
    window.draw(m_levelText);
}

sf::Vector2f ExperienceBar::getSize() const {
    return m_background.getSize();
}