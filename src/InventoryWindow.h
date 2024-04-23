#ifndef INVENTORY_WINDOW_H
#define INVENTORY_WINDOW_H

#include <SFML/Graphics.hpp>
#include <iostream>

class InventoryWindow {
public:
    InventoryWindow();

    void render(sf::RenderWindow& window);

    void setWeaponAvatarToCell(const sf::Sprite& weaponSprite);

private:
    sf::RectangleShape m_inventoryBackground;

    sf::Texture m_textureFirstRow; // Текстура для первого ряда

    sf::Sprite m_spritesFirstRow[8]; // Массив спрайтов для первого ряда

    std::vector<sf::Sprite> m_newSprites;
    int currentIndex = 0;
};

#endif // INVENTORY_WINDOW_H
