#include "InventoryWindow.h"

InventoryWindow::InventoryWindow() {
    m_inventoryBackground.setSize(sf::Vector2f(190, 40));
    m_inventoryBackground.setFillColor(sf::Color(100, 100, 100, 200));
    m_textureFirstRow.loadFromFile("../assets/textures/textures.png", sf::IntRect(80, 96, 16, 16));
    for (int i = 0; i < 8; ++i) {
        m_spritesFirstRow[i].setTexture(m_textureFirstRow);
        m_spritesFirstRow[i].setPosition(10 + i * 22, 10);
    }
}

void InventoryWindow::setWeaponAvatarToCell(const sf::Sprite& weaponSprite) {
    sf::Sprite newSprite = weaponSprite;
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

