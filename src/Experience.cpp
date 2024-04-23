#include "experience.h"

Experience::Experience(float x, float y) {
    m_shape.setRadius(5.0f);
    m_shape.setFillColor(sf::Color::Blue);
    m_shape.setPosition(x, y);
}

sf::Vector2f Experience::getPosition() {
    return m_shape.getPosition();
}

void Experience::render(sf::RenderWindow& window) {
    window.draw(m_shape);
}
