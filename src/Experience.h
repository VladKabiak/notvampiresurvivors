#ifndef EXPERIENCE_H
#define EXPERIENCE_H

#include <SFML/Graphics.hpp>

class Experience {
public:
    Experience(float x, float y);

    void render(sf::RenderWindow& window);

    sf::Vector2f getPosition();
private:

    sf::CircleShape m_shape;
};

#endif // EXPERIENCE_H
