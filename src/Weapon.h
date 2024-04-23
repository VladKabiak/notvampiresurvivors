#ifndef WEAPON_H
#define WEAPON_H

#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include "enemy.h"

class Weapon {
public:
    Weapon(const sf::Sprite& avatar, int damage, std::string name);

    bool canAttack() const;
    void resetAttackTimer();
    void updateAttackTimer();
    void attackEnemy(std::vector<Enemy>& enemies, const sf::Vector2f& characterPosition);
    const sf::Sprite &getAvatar();
    std::string getName();

private:
    const sf::Time ATTACK_INTERVAL = sf::seconds(2.0f);
    bool m_canAttack;
    sf::Sprite m_avatar;
    sf::Clock m_attackClock;
    const sf::Time m_attackInterval;
    int m_damage;
    std::string m_name;
};

#endif // WEAPON_H
