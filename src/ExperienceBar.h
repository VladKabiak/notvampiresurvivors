#ifndef EXPERIENCE_BAR_H
#define EXPERIENCE_BAR_H

#include <SFML/Graphics.hpp>
#include "Constants.h"

class ExperienceBar {
public:
    ExperienceBar(float width, float height);

    void setPosition(float x, float y);
    void setProgress(float progress);
    void render(sf::RenderWindow& window);
    sf::Vector2f getSize() const;
    float getProgress();
    void setLevel();
    void setLevel(int level);

private:
    sf::RectangleShape m_background;
    sf::RectangleShape m_fill;
    sf::Font m_levelFont;
    sf::Text m_levelText;
    float m_maxWidth;
    float progress{};
    int m_level = 0;
};

#endif // EXPERIENCE_BAR_H
