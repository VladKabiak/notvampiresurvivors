#include "InventoryWindow.h"

InventoryWindow::InventoryWindow(const float &WINDOW_WIDTH, const float &WINDOW_HEIGHT) {
    m_inventoryBackground.setSize(sf::Vector2f(WINDOW_WIDTH / 5, WINDOW_HEIGHT / 20));
    m_inventoryBackground.setFillColor(sf::Color(100, 100, 100, 200));
    m_textureFirstRow.loadFromFile(TEXTURES_PATH, sf::IntRect(80, 96, 16, 16));
    for (int i = 0; i < 8; ++i) {
        m_spritesFirstRow[i].setTexture(m_textureFirstRow);
        m_spritesFirstRow[i].setScale(2, 2);
        m_spritesFirstRow[i].setPosition(10 + i * 44, 10);
    }
}

void InventoryWindow::setWeaponAvatarToCell(const sf::Sprite& weaponSprite) {
    sf::Sprite newSprite = weaponSprite;
    newSprite.setScale(2, 2);
    newSprite.setPosition(m_spritesFirstRow[currentIndex].getPosition());
    m_newSprites.push_back(newSprite);
    currentIndex++;
}

void InventoryWindow::render(sf::RenderWindow& window) {
    window.draw(m_inventoryBackground);
    for (const auto& sprite : m_spritesFirstRow) {
        window.draw(sprite);
    }
    for (const auto& sprite : m_newSprites) {
        window.draw(sprite);
    }
}

