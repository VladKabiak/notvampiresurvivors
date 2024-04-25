#ifndef CHARACTER_H
#define CHARACTER_H

#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include "weapon.h"
#include "enemy.h"
#include "InventoryWindow.h"
#include "Constants.h"

class Character {
public:
    Character(float x, float y, int health, const sf::Font& font);
    void moveLeft(float distance);
    void moveRight(float distance);
    void moveUp(float distance);
    void moveDown(float distance);
    void update(float dt, std::vector<Enemy>& enemies);
    void takeDamage(int damage);
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
    int getHealth() const;
    void render(sf::RenderWindow& window);
    void setWeapon(Weapon* weapons, InventoryWindow &m_inventoryWindow);
    void setWeaponTexture(sf::Sprite& sprite);
    sf::Sprite getWeaponSprite();

private:
    sf::Sprite m_sprite;
    sf::Texture m_texture;
    std::vector<Weapon> m_weapons;
    sf::Sprite m_weaponSprite;
    sf::Text m_text;
    int m_health;
    int m_maxHealth;
    sf::Vector2f m_lastDirection;
    std::vector<sf::IntRect> m_animationFrames;
    unsigned int m_currentFrame;
    float m_animationTime;
    const float ANIMATION_DURATION;
    sf::Clock m_attackClock;
    sf::Time m_attackInterval;
    bool m_canAttack;
    bool isRight = true;
    bool isPrevRight = true;
    float m_attackDisplayTimer;
    const float ATTACK_DISPLAY_DURATION;
};


#endif // CHARACTER_H
