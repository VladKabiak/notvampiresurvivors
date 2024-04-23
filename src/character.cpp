#include "character.h"

Character::Character(float x, float y, int health, const sf::Font& font):
    m_maxHealth(health),
    m_weapons(),
    m_weaponSprite(),
    m_animationTime(0.0f),
    ANIMATION_DURATION(0.1f),
    ATTACK_DISPLAY_DURATION(0.1f) {

    m_sprite.setPosition(x, y);
    m_health = m_maxHealth;

    // Загрузка текстуры персонажа
    if (m_texture.loadFromFile(WALK_PATH)) {
        for (int i = 0; i < 8; ++i) {
            sf::IntRect frameRect(20 + i * 127, 50, 55, 77);
            m_animationFrames.push_back(frameRect);
        }

        m_currentFrame = 0;
        m_sprite.setTexture(m_texture);
        m_sprite.setTextureRect(m_animationFrames[m_currentFrame]);
        m_sprite.setPosition(x, y);
    }
}

void Character::setWeapon(Weapon* weapon,  InventoryWindow &m_inventoryWindow) {
    m_weapons.push_back(*weapon);
    m_inventoryWindow.setWeaponAvatarToCell(m_weapons.back().getAvatar());
}

sf::Sprite Character::getWeaponSprite() {
    return m_weaponSprite;
}

void Character::moveLeft(float distance) {
    m_sprite.move(-distance, 0);
    m_lastDirection = sf::Vector2f(-1.0f, 0.0f);
    isRight = false;
}

void Character::moveRight(float distance) {
    m_sprite.move(distance, 0);
    m_lastDirection = sf::Vector2f(1.0f, 0.0f);
    isRight = true;
}

void Character::moveUp(float distance) {
    m_sprite.move(0, -distance);
    m_lastDirection = sf::Vector2f(0.0f, -1.0f);
}

void Character::moveDown(float distance) {
    m_sprite.move(0, distance);
    m_lastDirection = sf::Vector2f(0.0f, 1.0f);
}

void Character::update(float dt, std::vector<Enemy>& enemies) {
    for (auto& weapon : m_weapons) {
        weapon.updateAttackTimer();
    }

    m_animationTime += dt;

    if (m_animationTime >= ANIMATION_DURATION) {
        m_currentFrame = (m_currentFrame + 1) % m_animationFrames.size();
        m_sprite.setTextureRect(m_animationFrames[m_currentFrame]);
        m_animationTime = 0.0f;
    }

    if (!isRight) {
        m_sprite.setScale(-1.f, 1.f);
    } else {
        m_sprite.setScale(1.f, 1.f);
    }

    if (m_weapons.front().canAttack()) {
        for (auto& weapon : m_weapons) {
            weapon.resetAttackTimer();
            weapon.attackEnemy(enemies, m_sprite.getPosition());
        }

        // Определяем угол поворота спрайта атаки в зависимости от направления
        float angle = 0.0f;
        if (m_lastDirection == sf::Vector2f(0.0f, 1.0f)) {
            angle = 90.0f;
        } else if (m_lastDirection == sf::Vector2f(-1.0f, 0.0f)) {
            angle = 180.0f;
        } else if (m_lastDirection == sf::Vector2f(0.0f, -1.0f)) {
            angle = 270.0f;
        } else if (m_lastDirection == sf::Vector2f(1.0f, 0.0f)) {
            angle = 0.f;
        }

        m_weaponSprite.setRotation(angle);

        m_weaponSprite.setPosition(m_sprite.getPosition());
        m_attackDisplayTimer = ATTACK_DISPLAY_DURATION;
    }

    if (m_attackDisplayTimer > 0.0f) {
        m_attackDisplayTimer -= dt;

        if (m_attackDisplayTimer <= 0.0f) {
            m_weaponSprite.setPosition(m_sprite.getPosition().x-1000, m_sprite.getPosition().y-1000);
        }
    }
}

void Character::takeDamage(int damage) {
    m_health -= damage;
}

void Character::setPosition(float x, float y) {
    m_sprite.setPosition(x, y);
}

sf::Vector2f Character::getPosition() const {
    return m_sprite.getPosition();
}

int Character::getHealth() const {
    return m_health;
}

void Character::render(sf::RenderWindow& window) {
    window.draw(m_sprite);

    sf::RectangleShape healthBarOutline(sf::Vector2f(60, 12));
    healthBarOutline.setFillColor(sf::Color::White);
    healthBarOutline.setOutlineColor(sf::Color::Black);
    healthBarOutline.setOutlineThickness(1.0f);

    float healthRatio = static_cast<float>(m_health) / static_cast<float>(m_maxHealth);

    float filledWidth = healthRatio * (healthBarOutline.getSize().x - 4);

    sf::RectangleShape healthBarFill(sf::Vector2f(filledWidth, 8));
    healthBarFill.setFillColor(sf::Color(181, 63, 101));

    healthBarOutline.setPosition(m_sprite.getPosition().x - healthBarOutline.getSize().x / 2, m_sprite.getPosition().y - 15);
    healthBarFill.setPosition(healthBarOutline.getPosition().x + 2, healthBarOutline.getPosition().y + 2);

    window.draw(healthBarOutline);
    window.draw(healthBarFill);
}

void Character::setWeaponTexture(sf::Sprite sprite) {
    m_weaponSprite = std::move(sprite);
}
