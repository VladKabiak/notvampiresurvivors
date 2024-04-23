#include "MainMenu.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "NE Vampire Survivors", sf::Style::Fullscreen);

    MainMenu mainMenu(window);
    mainMenu.run();

    return 0;
}
