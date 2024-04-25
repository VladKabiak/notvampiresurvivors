#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "character.h"
#include "enemy.h"
#include "Experience.h"
#include "ExperienceBar.h"
#include "MainMenu.h"
#include "Constants.h"

#include <cmath>
#include <cstdlib>

class Game {
public:
    explicit Game(sf::RenderWindow& window);

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
    std::vector<Experience> m_experience;
    ExperienceBar m_experienceBar;
    InventoryWindow m_inventoryWindow;
    bool m_running = true;
    sf::Texture m_textures;
    Weapon allWeapons[5] = {
            Weapon(sf::Sprite(m_textures, sf::IntRect(16, 80, 15, 15)), 8, "Knout"),
            Weapon(sf::Sprite(m_textures, sf::IntRect(32, 80, 15, 15)), 16, "Sphere"),
            Weapon(sf::Sprite(m_textures, sf::IntRect(48, 80, 15, 15)), 22, "Axe"),
            Weapon(sf::Sprite(m_textures, sf::IntRect(64, 80, 15, 15)), 26, "Club"),
            Weapon(sf::Sprite(m_textures, sf::IntRect(80, 80, 15, 15)), 7, "Ring")
    };
    float CHARACTER_SPEED = 0.2f;
    float GETTING_EXP_RADIUS = 50.f;
    float EXP_CELL = 0.1f;
    float CHAR_MAX_HP = 200.f;
    sf::Time ENEMY_SPAWN_INTERVAL = sf::seconds(0.5f);
    sf::RenderTexture m_bgImage;
    sf::Clock m_gameTimer;


    void createBgImage(sf::Sprite& sprite, sf::RenderTexture &BT, const float &WINDOW_WIDTH, const float &WINDOW_HEIGHT);
    void processEvents();
    void update();
    void render();
    bool collisionDetected(Character& character, Enemy& enemy);
    void gamePause();
    void openUpgradeWindow();
    void resumeGame();
    void applyUpgrade(const std::string &upgradeOption);
    void switchToMainMenu();
    void endGame();
    void renderUpgradeWindow(const sf::RectangleShape &upgradeWindow, const std::vector<std::string> &upgradeOptions);
    void handleUpgradeButtonClick(const int &mouseX, const int &mouseY, const std::vector<std::string> &upgradeOptions);
    std::vector<std::string> generateUpgradeOptions();
    sf::RectangleShape createUpgradeWindow();
    void drawButton(const sf::RectangleShape &button, const sf::Text &buttonText);
    sf::Text createButtonText(const std::string &text, const sf::RectangleShape &button);
    sf::RectangleShape createButton(float posX, float posY);
};

#endif // GAME_H