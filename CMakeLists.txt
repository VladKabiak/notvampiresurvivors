cmake_minimum_required(VERSION 3.16)
project(notvampiresurvivors-main)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)


add_executable(notvampiresurvivors-main
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
        src/Constants.h
)

target_link_libraries(notvampiresurvivors-main PRIVATE sfml-graphics)
target_compile_features(notvampiresurvivors-main PRIVATE cxx_std_17)

install(TARGETS notvampiresurvivors-main)