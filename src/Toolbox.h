#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "Button.h"

class Toolbox
{
    // private constructor
    Toolbox(){}
    static Toolbox* instance;

public:
    // Singleton Instance

    static Toolbox* getInstance();
    void releaseInstance();


    sf::RenderWindow win;     // SFML application window
    GameState* gameState;    // Primary game state representation
    Button* debugButton;     // Reveals mines in debug mode
    Button* newGameButton;   // Resets / starts new game
    Button* testButton1;     // Loads test board #1
    Button* testButton2;     // Loads test board #2
};

Toolbox *Toolbox::instance = nullptr;

Toolbox* Toolbox::getInstance()
{
    if (instance == nullptr)
        instance = new Toolbox();
    return instance;
}

void Toolbox::releaseInstance(){
    if(instance != nullptr)
        delete instance;
}