#include "MainMenu.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "NE Vampire Survivors");

    MainMenu mainMenu(window);
    mainMenu.run();

    return 0;
}
