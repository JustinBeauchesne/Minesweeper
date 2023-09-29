#pragma once
#include "Toolbox.h"
#include <fstream>


class Minesweeper
{
    // variables
    //std::vector<sf::Texture> spitter;
    sf::Event click;
    bool mode = false;
    Toolbox* tools = Toolbox::getInstance();
    GameState mainGame = GameState();
    GameState game1 = GameState("../images/testboard1.brd");
    GameState game2 = GameState("../images/testboard2.brd");
    GameState game3 = GameState("../images/testboard3.brd");


public:
    int launch();

    void restart();
    void render();

    void winLose();

    void toggleDebugMode();
    bool getDebugMode();

};

int Minesweeper::launch(){
    tools->win.create(sf::VideoMode{800,600}, "Loading...");
    tools->gameState = &mainGame;
    // for after initialization
    render();
    tools->win.setTitle("P4 - Minesweeper, Justin Beauchesne");

    while(tools->win.isOpen())
    {
        while (tools->win.pollEvent(click))
        {
            if (click.type == sf::Event::Closed)
            {
                tools->win.close();
            }

            else if (click.type == sf::Event::MouseButtonReleased)
            {
                float x = sf::Mouse::getPosition(tools->win).x / 32;
                float y = sf::Mouse::getPosition(tools->win).y / 32;
                Tile *selected = tools->gameState->getTile(x, y);

                // game tiles
                if(tools->gameState->getPlayStatus() == PLAYING)
                {
                    if (x < 25 && y < 16)
                    {
                        // Right mouse click;
                        if (click.mouseButton.button == sf::Mouse::Right)
                        {
                            if (selected->getState() == FLAGGED)
                            {
                                tools->gameState->setFlagCount(tools->gameState->getFlagCount() - 1);
                            }
                            else if (selected->getState() == HIDDEN)
                            {
                                tools->gameState->setFlagCount(tools->gameState->getFlagCount() + 1);
                            }
                            selected->onClickRight();
                            tools->gameState->displayFlags();

                            // Left mouse click
                        }
                        else if (click.mouseButton.button == sf::Mouse::Left)
                        {
                            if (!getDebugMode())
                            {
                                selected->onClickLeft();
                                if (selected->getState() == EXPLODED)
                                    tools->gameState->setPLayStatus(LOSS);
                            }
                            else
                                selected->onClickLeft();
                        }
                    }
                }
                    // smiley tile/ new game
                if ((10 < x && x < 14) && (y == 16 || y == 17))
                    restart();
                    // debug mode
                else if ((15 < x && x < 18) && (y == 16 || y == 17))
                {
                    if (getDebugMode())
                        mode = false;
                    else
                        mode = true;
                    toggleDebugMode();
                }
                    // game board 1
                else if ((17 < x && x < 20) && (y == 16 || y == 17))
                {
                    tools->gameState = &game1;
                }
                // game board 2
                else if ((19 < x && x < 22) && (y == 16 || y == 17))
                {
                    tools->gameState = &game2;
                }
                    // game baord 3
                else if ((21 < x && x < 24) && (y == 16 || y == 17))
                {
                    tools->gameState = &game3;
                }

                    // update
                winLose();
                std::cout<< tools->gameState->getPlayStatus()<<std::endl;
                if (tools->gameState->getPlayStatus() == WIN)
                {
                    mode = true;
                    toggleDebugMode();
                }
                else if (tools->gameState->getPlayStatus() == LOSS)
                {
                    mode = true;
                    toggleDebugMode();
                }
                //if (selected != nullptr)
                    render();
            }
        }
    }
    tools->releaseInstance();
    return 0;
}

void Minesweeper::restart(){
    tools->win.setTitle("Loading...");
    tools->gameState->setPLayStatus(PLAYING);
    mode = false;
    toggleDebugMode();
    tools->gameState = &mainGame;
    for(auto iter = tools->gameState->toSearch.begin(); iter != tools->gameState->toSearch.end(); iter++){
        iter->second->setBomb(false);
    }
    tools->gameState->randomBoard();
    tools->gameState->setFlagCount(0);
    tools->gameState->displayFlags();
    tools->win.setTitle("P4 - Minesweeper, Justin Beauchesne");
    tools->gameState->setPLayStatus(PLAYING);
}

void Minesweeper::render(){
    //iterate through map
    //tools->win.clear();
    for(auto iter = tools->gameState->toSearch.begin(); iter != tools->gameState->toSearch.end(); iter++)
    {
        tools->win.draw(iter->second->getSprite());
    }
    tools->win.display();
}

void Minesweeper::winLose(){

    int hidden = 0;
    for(auto iter = tools->gameState->toSearch.begin(); iter != tools->gameState->toSearch.end(); iter++)
    {
        if((iter->second->getLocation().x/32 < 25) && (iter->second->getLocation().y/32 < 16 ))
        {
            if (iter->second->getState() == HIDDEN)
            {
                hidden++;
                if (hidden > tools->gameState->getMineCount())
                    break;
            }
        }
    }

    if(hidden == tools->gameState->getMineCount())
        tools->gameState->setPLayStatus(WIN);
    render();
}

void Minesweeper::toggleDebugMode(){
    if(getDebugMode()){
        for(auto iter = tools->gameState->toSearch.begin(); iter != tools->gameState->toSearch.end(); iter++)
        {
            if((iter->second->getLocation().x/32 < 25) && (iter->second->getLocation().y/32 < 16 ))
            {
                iter->second->setState(REVEALED, true);
            }
        }
    }
    else{
        for(auto iter = tools->gameState->toSearch.begin(); iter != tools->gameState->toSearch.end(); iter++)
        {
            if(iter->second->getLocation().x/32 < 25 && iter->second->getLocation().y/32 < 16 )
            {
                iter->second->setState(HIDDEN);
            }
        }
    }
    render();
}

bool Minesweeper::getDebugMode(){
    return mode;
}

