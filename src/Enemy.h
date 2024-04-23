#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <cmath>

class Enemy {
public:
    Enemy(float startX, float startY, int health, const sf::Texture &texture);

    void update(const sf::Vector2f& targetPosition, float dt);
    sf::Sprite getSprite() const;
    const sf::Vector2f &getPosition();
    void takeDamage(int damage);
    int getHealth() const;
    bool canAttack();

private:
    sf::CircleShape m_shape;
    float m_speed = 0.03f;
    int m_health;
    sf::Text m_text;
    float m_lastDamageTime;
    float DAMAGE_COOLDOWN;
    sf::Sprite m_sprite;
};

#endif // ENEMY_H
