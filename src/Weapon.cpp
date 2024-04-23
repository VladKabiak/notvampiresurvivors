#include "weapon.h"

const sf::Time ATTACK_INTERVAL = sf::seconds(2.0f);

Weapon::Weapon(const sf::Sprite& avatar, int damage, std::string name)
        : m_canAttack(true), m_attackClock(), m_attackInterval(ATTACK_INTERVAL), m_avatar(avatar), m_damage(damage), m_name(name) {}

bool Weapon::canAttack() const {
    return m_canAttack;
}

const sf::Sprite& Weapon::getAvatar() {
    return m_avatar;
}

void Weapon::resetAttackTimer() {
    m_attackClock.restart();
    m_canAttack = false;
}

void Weapon::updateAttackTimer(float dt) {
    if (m_attackClock.getElapsedTime() >= m_attackInterval) {
        m_canAttack = true;
    }
}

std::string Weapon::getName() {
    return m_name;
}

void Weapon::attackEnemy(std::vector<Enemy>& enemies, const sf::Vector2f& characterPosition) {
    // Для простоты демонстрации, здесь происходит атака по всем врагам на экране
    for (auto& enemy : enemies) {
        // Проверяем расстояние между персонажем и врагом
        float distance = sqrt(pow(characterPosition.x - enemy.getPosition().x, 2) +
                              pow(characterPosition.y - enemy.getPosition().y, 2));
        if (distance <= 238) {
            enemy.takeDamage(m_damage);
        }
    }
}
