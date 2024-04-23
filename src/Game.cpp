#include <sstream>
#include <iostream>
#include <cmath>
#include "game.h"
#include "MainMenu.h"
#include <cstdlib>

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
float CHARACTER_SPEED = 0.2f;
float GETTING_EXP_RADIUS = 50.f;
float EXP_CELL = 0.1f;
float CHAR_MAX_HP = 200.f;
const sf::Time ENEMY_SPAWN_INTERVAL = sf::seconds(0.1f);

sf::Texture Game::m_textures;

Weapon Game::allWeapons[5] = {
        Weapon(sf::Sprite (m_textures, sf::IntRect (16, 80, 15, 15) ), 10, "Knout"),
        Weapon(sf::Sprite (m_textures, sf::IntRect (32, 80, 15, 15) ), 20, "Sphere"),
        Weapon(sf::Sprite (m_textures, sf::IntRect (48, 80, 15, 15) ), 30, "Axe"),
        Weapon(sf::Sprite (m_textures, sf::IntRect (64, 80, 15, 15) ), 40, "Club"),
        Weapon(sf::Sprite (m_textures, sf::IntRect (80, 80, 15, 15) ), 5, "Ring")
};

Game::Game(sf::RenderWindow &window) : m_window(window), m_character(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2,
                                                                     CHAR_MAX_HP, m_font),
                                                                     m_experienceBar(250,20), m_inventoryWindow()
                                                                     {
    // Инициализация камеры
    m_view = m_window.getDefaultView();

    m_hudView = m_window.getDefaultView();

    m_font.loadFromFile("../assets/fonts/arial.ttf");

    if (!m_textures.loadFromFile("../assets/textures/textures.png")) {
        std::cerr << "Failed to load knout attack texture!" << std::endl;
    } else {
        sf::Sprite knoutAttackSprite(m_textures, sf::IntRect(145, 0, 238, 30) );
        m_character.setWeaponTexture(knoutAttackSprite);
    }

    m_experienceBar.setPosition((WINDOW_WIDTH - m_experienceBar.getSize().x) / 2, 50);
    m_character.setWeapon(allWeapons, m_inventoryWindow);
    m_running = true;
}

void Game::run() {
    while (m_window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            m_window.close();
    }
}

void Game::update() {
    if (m_running) {
        float dt = m_clock.restart().asSeconds();
        m_character.update(dt, m_enemies);

        // Обработка перемещения персонажа
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            m_character.moveLeft(CHARACTER_SPEED);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            m_character.moveRight(CHARACTER_SPEED);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            m_character.moveUp(CHARACTER_SPEED);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            m_character.moveDown(CHARACTER_SPEED);
        }

        // Проверяем столкновения и нанесение урона персонажу
        for (auto& enemy : m_enemies) {
            if (collisionDetected((Character &) m_character, enemy) && enemy.canAttack()) {
                if (m_character.getHealth() > 10) {
                    m_character.takeDamage(10); // Уменьшаем здоровье персонажа на 10
                } else {
                    endGame();
                }
            }
        }

        // Проверяем подбор опыта
        m_experience.erase(std::remove_if(m_experience.begin(), m_experience.end(), [&](Experience& exp) {
            sf::Vector2f characterPosition = m_character.getPosition();
            sf::Vector2f expPosition = exp.getPosition();

            float distance = std::sqrt(std::pow(characterPosition.x - expPosition.x, 2) + std::pow(characterPosition.y - expPosition.y, 2));

            if (distance <= GETTING_EXP_RADIUS) {
                if (m_experienceBar.getProgress() + EXP_CELL >= 1){
                    m_experienceBar.setProgress(0);
                    gamePause();
                    openUpgradeWindow();
                }
                m_experienceBar.setProgress(m_experienceBar.getProgress() + EXP_CELL);
                return true; // Удаляем опыт из вектора
            } else {
                return false; // Оставляем опыт в векторе
            }
        }), m_experience.end());


        // Обновляем позиции врагов
        for (auto& enemy : m_enemies) {
            enemy.update(m_character.getPosition(), dt);
        }

        // Проверяем смерть врагов и создаем опыт при смерти
        for (auto it = m_enemies.begin(); it != m_enemies.end(); ++it) {
            if (it->getHealth() <= 0) {
                if (rand() % 100 < 50) { // 50% шанс выпадения опыта
                    m_experience.emplace_back(it->getPosition().x, it->getPosition().y);
                }
                m_enemies.erase(it);
                break;
            }
        }


        if (m_enemySpawnClock.getElapsedTime() >= ENEMY_SPAWN_INTERVAL) {
            float angle = static_cast<float>(rand() % 360);
            float radius = 1000.0f;
            float x = m_character.getPosition().x + radius * std::cos(angle);
            float y = m_character.getPosition().y + radius * std::sin(angle);

            m_enemies.emplace_back(x, y, 10, m_textures);

            m_enemySpawnClock.restart();
        }

        m_view.setCenter(m_character.getPosition());
    }
}


void Game::render() {
    if (m_running) {
        m_window.clear();

        m_window.setView(m_view);
        sf::RenderTexture backgroundTexture;
        backgroundTexture.create(WINDOW_WIDTH, WINDOW_HEIGHT);

        sf::Sprite sprite;
        sprite.setTexture(m_textures);
        sprite.setTextureRect(sf::IntRect(96, 63, 65, 65));

        int repeatX = WINDOW_WIDTH / 60;
        int repeatY = WINDOW_HEIGHT / 60;

        for (int i = 0; i < repeatX; ++i) {
            for (int j = 0; j < repeatY; ++j) {
                sprite.setPosition(i * 60, j * 60);
                backgroundTexture.draw(sprite);
            }
        }

        backgroundTexture.display();
        sf::Sprite backgroundSprite(backgroundTexture.getTexture());

        m_window.draw(backgroundSprite);


        m_character.render(m_window);

        m_window.draw(m_character.getWeaponSprite());

        for (Enemy enemy : m_enemies) {
            m_window.draw(enemy.getSprite());
        }

        for (auto& exp : m_experience) {
            exp.render(m_window);
        }

        m_window.setView(m_hudView);
        m_experienceBar.render(m_window);
        m_inventoryWindow.render(m_window);

        m_window.display();
    }
}

bool Game::collisionDetected(Character &character, Enemy &enemy) {
    sf::Vector2f characterPosition = character.getPosition();
    sf::Vector2f enemyPosition = enemy.getPosition();

    // Вычисляем расстояние между центрами персонажа и врага
    float distance = std::sqrt(std::pow(characterPosition.x - enemyPosition.x, 2) + std::pow(characterPosition.y - enemyPosition.y, 2));

    if (distance <= 100) {
        return true;
    }

    return false;
}

void Game::gamePause() {
    m_running = false;
}

void Game::resumeGame() {
    m_running = true;
}

void Game::openUpgradeWindow() {
    sf::RectangleShape upgradeWindow(sf::Vector2f(350, 400));
    upgradeWindow.setFillColor(sf::Color(30, 30, 30, 200));
    upgradeWindow.setOutlineColor(sf::Color::White);
    upgradeWindow.setOutlineThickness(2.0f);

    upgradeWindow.setOrigin(175, 200);
    upgradeWindow.setPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);

    sf::Text buttonText;
    buttonText.setFont(m_font);
    buttonText.setCharacterSize(12);
    buttonText.setFillColor(sf::Color::White);

    std::string upgradeOptions[3];

    for (int i = 0; i < 3; ++i) {
        int randomUpgrade = rand() % 5; // Generate a random number between 0 and 4
        switch (randomUpgrade) {
            case 0:
                upgradeOptions[i] = "Upgrade CHAR_MAX_HP on 10%";
                break;
            case 1:
                upgradeOptions[i] = "Upgrade EXP_CELL on 10%";
                break;
            case 2:
                upgradeOptions[i] = "Upgrade GETTING_EXP_RADIUS on 5";
                break;
            case 3:
                upgradeOptions[i] = "Upgrade CHARACTER_SPEED on 10%";
                break;
            case 4:
                int randomWeaponIndex = rand() % 5;
                upgradeOptions[i] = "Get a new weapon - " + allWeapons[randomWeaponIndex].getName();
                break;
        }
    }

    sf::RectangleShape button(sf::Vector2f(250, 50));
    button.setFillColor(sf::Color (74, 140, 116, 255));
    button.setOutlineColor(sf::Color::White);
    button.setOutlineThickness(2.0f);
    button.setOrigin(125, 25);

    m_window.draw(upgradeWindow);

    for (int i = 0; i < 3; ++i) {
        button.setPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f - 100 + i * 100);
        buttonText.setString(upgradeOptions[i]);
        buttonText.setPosition(button.getPosition().x - 100, button.getPosition().y - 12);

        m_window.draw(button);

        m_window.draw(buttonText);
    }

    m_window.display();

    sf::Event event;
    while (m_window.waitEvent(event)) {
        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = m_window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
            for (int i = 0; i < 3; ++i) {
                button.setPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f - 100 + i * 100);
                if (button.getGlobalBounds().contains(mousePos)) {
                    applyUpgrade(upgradeOptions[i]);
                    resumeGame();
                    return;
                }
            }
        }
    }
}

void Game::applyUpgrade(const std::string& upgradeOption) {
    if (upgradeOption == "Upgrade CHAR_MAX_HP on 10%") {
        CHAR_MAX_HP *= 1.1f;
    } else if (upgradeOption == "Upgrade EXP_CELL on 10%") {
        EXP_CELL *= 1.1f;
    } else if (upgradeOption == "Upgrade GETTING_EXP_RADIUS on 5") {
        GETTING_EXP_RADIUS += 5.0f;
    } else if (upgradeOption == "Upgrade CHARACTER_SPEED on 10%") {
        CHARACTER_SPEED *= 1.1f;
    } else if (upgradeOption.find("Get a new weapon - ") != std::string::npos) {
        std::string weaponName = upgradeOption.substr(19);
        for (auto &weapon: allWeapons) {
            if (weapon.getName() == weaponName) {
                m_character.setWeapon(&weapon, m_inventoryWindow);
                break;
            }
        }
    }
}

void Game::switchToMainMenu() {
    MainMenu mainMenu(m_window);
    mainMenu.run();
}

void Game::endGame() {
    switchToMainMenu();
}
