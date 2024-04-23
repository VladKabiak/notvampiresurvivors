#include "game.h"

Game::Game(sf::RenderWindow &window) :
        m_window(window),
        m_character(m_window.getSize().x / 2, m_window.getSize().y / 2, 20, m_font),
        m_experienceBar(m_window.getSize().x / 7, 20),
        m_inventoryWindow(),
        m_view(m_window.getDefaultView()),
        m_hudView(m_window.getDefaultView()) {

    m_font.loadFromFile(FONT_PATH);

    if (m_textures.loadFromFile(TEXTURES_PATH)) {
        sf::Sprite knoutAttackSprite(m_textures, sf::IntRect(145, 0, 238, 30) );
        m_character.setWeaponTexture(knoutAttackSprite);
    }

    m_experienceBar.setPosition((m_window.getSize().x - m_experienceBar.getSize().x) / 2, 50);
    m_character.setWeapon(allWeapons, m_inventoryWindow);
}

void Game::run() {
    while (m_window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents() {
    sf::Event event{};
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
                    m_character.takeDamage(10);
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
        std::for_each(m_enemies.begin(), m_enemies.end(), [&](Enemy& enemy) {
            enemy.update(m_character.getPosition(), dt);
        });

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
            auto angle = static_cast<float>(rand() % 360);
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
        backgroundTexture.create(m_window.getSize().x, m_window.getSize().y);

        sf::Sprite sprite;
        sprite.setTexture(m_textures);
        sprite.setTextureRect(sf::IntRect(96, 63, 65, 65));

        int repeatX = m_window.getSize().x / 60;
        int repeatY = m_window.getSize().y / 60;

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
    float distance = std::sqrt(std::pow(characterPosition.x - enemyPosition.x, 2) + std::pow(characterPosition.y - enemyPosition.y, 2));
    return distance <= 100;
}

void Game::openUpgradeWindow() {
    sf::RectangleShape upgradeWindow = createUpgradeWindow();
    std::vector<std::string> upgradeOptions = generateUpgradeOptions();

    while (true) {
        renderUpgradeWindow(upgradeWindow, upgradeOptions);

        sf::Event event;
        while (m_window.waitEvent(event)) {
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                handleUpgradeButtonClick(event.mouseButton.x, event.mouseButton.y, upgradeOptions);
                return;
            }
        }
    }
}

void Game::applyUpgrade(const std::string& upgradeOption) {
    if (upgradeOption == "Increase character's maximum health by 10%") {
        CHAR_MAX_HP *= 1.1f;
    } else if (upgradeOption == "Increase experience gained by 7%") {
        EXP_CELL *= 1.07f;
    } else if (upgradeOption == "Increase collecting experience radius by 5") {
        GETTING_EXP_RADIUS += 5.0f;
    } else if (upgradeOption == "Increase character speed by 11%") {
        CHARACTER_SPEED *= 1.11f;
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

void Game::gamePause() {
    m_running = false;
}

void Game::resumeGame() {
    m_running = true;
}

void Game::endGame() {
    gamePause();

    sf::RectangleShape gameOverWindow(sf::Vector2f(m_window.getSize().x, m_window.getSize().y));
    gameOverWindow.setFillColor(sf::Color(30, 30, 30, 200)); // Серый цвет с прозрачностью
    gameOverWindow.setPosition(0, 0);

    sf::Font font;
    font.loadFromFile(FONT_PATH);

    sf::Text gameOverText("GAME OVER", font, 60);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setStyle(sf::Text::Bold);
    gameOverText.setPosition((m_window.getSize().x - gameOverText.getGlobalBounds().width) / 2,
                             (m_window.getSize().y - gameOverText.getGlobalBounds().height) / 2 - 100);

    sf::RectangleShape mainMenuButton = createButton((m_window.getSize().x - 200) / 2, (m_window.getSize().y - 50) / 2 + 50);
    sf::Text mainMenuButtonText = createButtonText("Main Menu", mainMenuButton);

    sf::RectangleShape exitButton = createButton( (m_window.getSize().x - 200) / 2, (m_window.getSize().y - 50) / 2 + 150);
    sf::Text exitButtonText = createButtonText("Exit Game", exitButton);

    while (m_window.isOpen()) {
        sf::Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                m_window.close();
            else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = m_window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                if (mainMenuButton.getGlobalBounds().contains(mousePos)) {
                    switchToMainMenu();
                    return;
                } else if (exitButton.getGlobalBounds().contains(mousePos)) {
                    m_window.close();
                    return;
                }
            }
        }

        m_window.draw(gameOverWindow);
        m_window.draw(gameOverText);
        drawButton(mainMenuButton, mainMenuButtonText);
        drawButton(exitButton, exitButtonText);
        m_window.display();
    }
}

void Game::switchToMainMenu() {
    MainMenu mainMenu(m_window);
    mainMenu.run();
}

sf::RectangleShape Game::createButton(float posX, float posY) {
    sf::RectangleShape button(sf::Vector2f(200, 50));
    button.setFillColor(sf::Color(74, 140, 116, 255));
    button.setPosition(posX, posY);
    return button;
}

sf::Text Game::createButtonText(const std::string& text, const sf::RectangleShape& button) {
    sf::Text buttonText(text, m_font, 20);
    buttonText.setFillColor(sf::Color::White);
    buttonText.setPosition(button.getPosition().x + (button.getSize().x - buttonText.getGlobalBounds().width) / 2,
                           button.getPosition().y + (button.getSize().y - buttonText.getGlobalBounds().height) / 2);
    return buttonText;
}

void Game::drawButton(const sf::RectangleShape& button, const sf::Text& buttonText) {
    m_window.draw(button);
    m_window.draw(buttonText);
}

sf::RectangleShape Game::createUpgradeWindow() {
    sf::RectangleShape upgradeWindow(sf::Vector2f(m_window.getSize().x / 4, m_window.getSize().y / 1.7));
    upgradeWindow.setFillColor(sf::Color(30, 30, 30, 200));
    upgradeWindow.setOutlineColor(sf::Color::White);
    upgradeWindow.setOutlineThickness(2.0f);
    upgradeWindow.setOrigin(m_window.getSize().x / 8, m_window.getSize().y / 3.4);
    upgradeWindow.setPosition(m_window.getSize().x / 2.0f, m_window.getSize().y / 2.0f);
    return upgradeWindow;
}

std::vector<std::string> Game::generateUpgradeOptions() {
    std::vector<std::string> upgradeOptions;
    for (int i = 0; i < 3; ++i) {
        int randomUpgrade = rand() % 5;
        switch (randomUpgrade) {
            case 0:
                upgradeOptions.emplace_back("Increase character's maximum health by 10%");
                break;
            case 1:
                upgradeOptions.emplace_back("Increase experience gained by 7%");
                break;
            case 2:
                upgradeOptions.emplace_back("Increase collecting experience radius by 5");
                break;
            case 3:
                upgradeOptions.emplace_back("Increase character speed by 11%");
                break;
            case 4: {
                int randomWeaponIndex = rand() % 5;
                upgradeOptions.push_back("Get a new weapon - " + allWeapons[randomWeaponIndex].getName());
                break;
            }
        }
    }
    return upgradeOptions;
}

void Game::renderUpgradeWindow(const sf::RectangleShape& upgradeWindow, const std::vector<std::string>& upgradeOptions) {
    m_window.draw(upgradeWindow);

    sf::RectangleShape button(sf::Vector2f(m_window.getSize().x / 6, m_window.getSize().y / 16));
    button.setFillColor(sf::Color(74, 140, 116, 255));
    button.setOutlineColor(sf::Color::White);
    button.setOutlineThickness(2.0f);
    button.setOrigin(m_window.getSize().x / 12, m_window.getSize().y / 32);

    sf::Text buttonText;
    buttonText.setFont(m_font);
    buttonText.setCharacterSize(14);
    buttonText.setFillColor(sf::Color::White);

    for (size_t i = 0; i < upgradeOptions.size(); ++i) {
        button.setPosition(m_window.getSize().x / 2.0f, m_window.getSize().y / 2.0f - 100 + i * 100);
        buttonText.setString(upgradeOptions[i]);
        buttonText.setPosition(button.getPosition().x - m_window.getSize().x / 13, button.getPosition().y - 5);

        m_window.draw(button);
        m_window.draw(buttonText);
    }

    m_window.display();
}

void Game::handleUpgradeButtonClick(const int &mouseX, const int &mouseY, const std::vector<std::string>& upgradeOptions) {
    sf::Vector2f mousePos = m_window.mapPixelToCoords(sf::Vector2i(mouseX, mouseY));
    sf::RectangleShape button(sf::Vector2f(m_window.getSize().x / 6, m_window.getSize().y / 16));
    button.setOrigin(m_window.getSize().x / 12, m_window.getSize().y / 32);

    for (size_t i = 0; i < upgradeOptions.size(); ++i) {
        button.setPosition(m_window.getSize().x / 2.0f, m_window.getSize().y / 2.0f - 100 + i * 100);
        if (button.getGlobalBounds().contains(mousePos)) {
            applyUpgrade(upgradeOptions[i]);
            resumeGame();
            return;
        }
    }
}
