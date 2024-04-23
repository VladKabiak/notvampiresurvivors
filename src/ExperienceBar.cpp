#include "experiencebar.h"

ExperienceBar::ExperienceBar(float width, float height)
        : m_background(sf::Vector2f(width, height)),
          m_fill(sf::Vector2f(0, height)), m_maxWidth(width) {
    m_background.setFillColor(sf::Color::White);
    m_background.setOutlineColor(sf::Color::Black);
    m_background.setOutlineThickness(2.0f);

    m_fill.setFillColor(sf::Color::Blue);
}

void ExperienceBar::setPosition(float x, float y) {
    m_background.setPosition(x, y);
    m_fill.setPosition(x, y);
}

float ExperienceBar::getProgress() {
    return progress;
}

void ExperienceBar::setProgress(float progr) {
    // Прогресс должен быть в диапазоне от 0 до 1
    progress = progr;

    // Изменяем ширину заполнения пропорционально прогрессу
    float fillWidth = m_maxWidth * progress;
    m_fill.setSize(sf::Vector2f(fillWidth, m_background.getSize().y));
}

void ExperienceBar::render(sf::RenderWindow& window) {
    window.draw(m_background);
    window.draw(m_fill);
}

sf::Vector2f ExperienceBar::getSize() const {
    return m_background.getSize();
}