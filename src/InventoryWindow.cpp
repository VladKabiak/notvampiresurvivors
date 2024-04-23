#include "InventoryWindow.h"

InventoryWindow::InventoryWindow() {
    // Инициализация заднего фона инвентаря
    m_inventoryBackground.setSize(sf::Vector2f(190, 40));
    m_inventoryBackground.setFillColor(sf::Color(100, 100, 100, 200)); // Серый с небольшой прозрачностью

    // Загрузка текстуры для первого ряда
    if (!m_textureFirstRow.loadFromFile("../assets/textures/textures.png", sf::IntRect(80, 96, 16, 16))) {
        std::cerr << "Failed to load texture for the first row!" << std::endl;
    }

    // Инициализация спрайтов в первом ряду
    for (int i = 0; i < 8; ++i) {
        m_spritesFirstRow[i].setTexture(m_textureFirstRow); // Установка текстуры для спрайта
        m_spritesFirstRow[i].setPosition(10 + i * 22, 10); // Позиционирование спрайтов
    }
}

void InventoryWindow::setWeaponAvatarToCell(const sf::Sprite& weaponSprite) {
    sf::Sprite newSprite = weaponSprite;
    newSprite.setPosition(m_spritesFirstRow[currentIndex].getPosition());
    m_newSprites.push_back(newSprite);
    currentIndex++;
}

void InventoryWindow::render(sf::RenderWindow& window) {
    // Отрисовка заднего фона инвентаря
    window.draw(m_inventoryBackground);

    // Отрисовка спрайтов для первого ряда
    for (const auto& sprite : m_spritesFirstRow) {
        window.draw(sprite);
    }

    // Отрисовка новых спрайтов
    for (const auto& sprite : m_newSprites) {
        window.draw(sprite);
    }
}

