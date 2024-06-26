#ifndef INVENTORY_WINDOW_H
#define INVENTORY_WINDOW_H

#include <SFML/Graphics.hpp>
#include "Constants.h"

class InventoryWindow {
public:
    InventoryWindow(const float &WINDOW_WIDTH, const float &WINDOW_HEIGHT);

    void render(sf::RenderWindow& window);

    void setWeaponAvatarToCell(const sf::Sprite& weaponSprite);

private:
    sf::RectangleShape m_inventoryBackground;

    sf::Texture m_textureFirstRow;

    sf::Sprite m_spritesFirstRow[8];

    std::vector<sf::Sprite> m_newSprites;
    int currentIndex = 0;
};

#endif // INVENTORY_WINDOW_H
