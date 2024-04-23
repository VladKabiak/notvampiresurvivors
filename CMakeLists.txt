cmake_minimum_required(VERSION 3.28)
project(untitled3)

include(FetchContent)
set(BUILD_SHARED_LIBS OFF)
FETCHCONTENT_DECLARE(SFML GIT_REPOSITORY https://github.com/SFML/SFML.git GIT_TAG 2.5.1)
FETCHCONTENT_MAKEAVAILABLE(SFML)

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled3
        src/main.cpp
        src/Game.cpp
        src/Game.h
        src/character.cpp
        src/character.h
        src/Enemy.cpp
        src/Enemy.h
        src/Weapon.cpp
        src/Weapon.h
        src/Experience.cpp
        src/Experience.h
        src/ExperienceBar.cpp
        src/ExperienceBar.h
        src/InventoryWindow.cpp
        src/InventoryWindow.h
        src/MainMenu.cpp
        src/MainMenu.h
        # Добавьте здесь Game.cpp
)

target_link_libraries(untitled3 sfml-graphics)
