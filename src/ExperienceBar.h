#ifndef EXPERIENCE_BAR_H
#define EXPERIENCE_BAR_H

#include <SFML/Graphics.hpp>

class ExperienceBar {
public:
    ExperienceBar(float width, float height);

    void setPosition(float x, float y);
    void setProgress(float progress);
    void render(sf::RenderWindow& window);
    sf::Vector2f getSize() const;
    float getProgress();

private:
    sf::RectangleShape m_background; // Фон опыта
    sf::RectangleShape m_fill;       // Заполнение опыта
    float m_maxWidth;                 // Максимальная ширина шкалы опыта
    float progress{};
};

#endif // EXPERIENCE_BAR_H
