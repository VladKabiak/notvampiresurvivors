#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <cmath>
#include "Constants.h"

class Enemy {
public:
    Enemy(float startX, float startY, int health, const sf::Texture &texture);

    void update(const sf::Vector2f& targetPosition, float dt);
    const sf::Vector2f &getPosition();
    void takeDamage(int damage);
    int getHealth() const;
    bool canAttack();
    void render(sf::RenderWindow &window);

private:
    sf::CircleShape m_shape;
    float m_speed = 0.03f;
    int m_health;
    float m_lastDamageTime;
    float m_lastTakenDamageTime;
    int  m_lastDamage;
    float DAMAGE_COOLDOWN;
    sf::Sprite m_sprite;
    sf::Font m_font;
    sf::Text m_damageText;
};

#endif // ENEMY_H
