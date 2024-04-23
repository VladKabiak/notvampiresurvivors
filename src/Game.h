#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "character.h"
#include "enemy.h"
#include "Experience.h"
#include "ExperienceBar.h"

class Game {
public:
    Game(sf::RenderWindow& window);

    void run();

private:
    sf::RenderWindow& m_window;
    Character m_character;
    std::vector<Enemy> m_enemies;
    sf::View m_view;
    sf::View m_hudView;
    sf::Font m_font;
    sf::Text m_coordinatesText;
    sf::Clock m_enemySpawnClock;
    sf::Clock m_clock;
    static sf::Texture m_textures;
    std::vector<Experience> m_experience;
    ExperienceBar m_experienceBar;
    InventoryWindow m_inventoryWindow;
    bool m_running;
    static Weapon allWeapons[5];



    void processEvents();
    void update();
    void render();
    bool collisionDetected(Character &character, Enemy &enemy);
    void gamePause();
    void openUpgradeWindow();
    void resumeGame();

    void applyUpgrade(const std::string &upgradeOption);

    void switchToMainMenu();

    void endGame();
};

#endif // GAME_H
