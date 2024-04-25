#include <iostream>
#include "enemy.h"

Enemy::Enemy(float startX, float startY, int health, const sf::Texture &texture):
    m_health(health),
    m_lastDamage (),
    m_lastDamageTime(),
    m_lastTakenDamageTime(999.0f),
    DAMAGE_COOLDOWN(1.0f),
    m_sprite() {

    sf::IntRect enemyRegions[4] = {
            sf::IntRect(164, 34, 42, 30),
            sf::IntRect(210, 34, 40, 29),
            sf::IntRect(255, 40, 48, 40),
            sf::IntRect(384, 0, 31, 31)
    };

    m_sprite.setTexture(texture);
    m_sprite.setTextureRect(enemyRegions[rand() % 4]);
    m_sprite.setPosition(startX, startY);

    m_font.loadFromFile(FONT_PATH);
    m_damageText.setCharacterSize(20);
    m_damageText.setFillColor(sf::Color::Red);
}

void Enemy::update(const sf::Vector2f& targetPosition, float dt) {
    sf::Vector2f direction = targetPosition - m_sprite.getPosition();
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);

    direction /= length;

    m_sprite.move(direction * m_speed);

    m_lastDamageTime += dt;
    m_lastTakenDamageTime += dt;
}

void Enemy::render(sf::RenderWindow &window){
    window.draw(m_sprite);
    if (m_lastTakenDamageTime < 0.5f){
        m_damageText.setFont(m_font);
        m_damageText.setString("-" + std::to_string(m_lastDamage));
        m_damageText.setPosition(m_sprite.getPosition().x + 5, m_sprite.getPosition().y - 30);
        window.draw(m_damageText);
    } else {
        m_damageText.setString("");
    }
}

void Enemy::takeDamage(int damage) {
    m_health -= damage;
    if (m_health > 0) {
        m_lastDamage = damage;
        m_lastTakenDamageTime = 0.0f;
    }
}

int Enemy::getHealth() const {
    return m_health;
}

bool Enemy::canAttack() {
    if (m_lastDamageTime >= DAMAGE_COOLDOWN) {
        m_lastDamageTime = 0.0f;
        return true;
    } else {
        return false;
    }
}

const sf::Vector2f & Enemy::getPosition() {
    return m_sprite.getPosition();
}