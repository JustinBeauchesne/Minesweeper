#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include "Button.h"
#include "Toolbox.h"

enum State { REVEALED, HIDDEN, FLAGGED, EXPLODED, ISFLAG };
//ToolBox* tools = Toolbox::getInstance();

class Tile
{
    // variables
    friend class GameState;
    State currentState;
    sf::Texture temp;
    sf::Texture overlay;
    bool Bomb;
    sf::Sprite tilePic;
    std::array<Tile*, 8> neighbors;
public:
    // Tile constructor
    Tile(sf::Vector2f position);

    // Sprite getter
    sf::Sprite getSprite();
        // tile location getter
    sf::Vector2f getLocation();

    // current tile state getter and setter
    State getState();
    void setState(State _state, bool debug);

    // Update bomb state
    bool getBomb();
    void setBomb(bool test);

    // tile neighbors getter and setters
    std::array<Tile*, 8>& getNeighbors();

    // Click actions
    void onClickLeft();
    void onClickRight();

    // Update sprite
    void draw();

protected:
    void revealNeighbors();

};

Tile::Tile(sf::Vector2f position){
// Set default bomb, position, and button attributes
//Button tileButton(position, // click function?);
    Bomb = false;
    temp.loadFromFile("../images/tile_hidden.png");
    tilePic.setTexture(temp);
    tilePic.setPosition(position.x * 32, position.y * 32);
    currentState = HIDDEN;
}

sf::Sprite Tile::getSprite(){
    return this->tilePic;
}

// tile location getter
sf::Vector2f Tile::getLocation(){
    return Tile::tilePic.getPosition();
}

// current tile state getter and setter
State Tile::getState(){
    return currentState;
}
void Tile::setState(State _state, bool debug = false)
{
        currentState = _state;
        if (_state == HIDDEN)
        {
            //temp.loadFromFile("../images/tile_hidden.png");
            overlay.loadFromFile("../images/tile_hidden.png");
            temp.update(overlay);
            tilePic.setTexture(temp);
        }
        else if (_state == FLAGGED)
        {
            temp.loadFromFile("../images/flag.png");
            tilePic.setTexture(temp);
        }
        else if (_state == REVEALED)
        {
            temp.loadFromFile("../images/tile_revealed.png");
            tilePic.setTexture(temp);

            if (getBomb())
            {
                overlay.loadFromFile("../images/mine.png");
                temp.update(overlay);
                if(!debug)
                    currentState = EXPLODED;
            }
            else if (!getBomb())
            {

                int count = 0;
                for (int i = 0; i < 8; i++)
                {
                    if (neighbors[i] != nullptr)
                    {
                        if (neighbors[i]->getBomb())
                            count++;
                    }
                }
                if (count == 1)
                    overlay.loadFromFile("../images/number_1.png");

                else if (count == 2)
                    overlay.loadFromFile("../images/number_2.png");

                else if (count == 3)
                    overlay.loadFromFile("../images/number_3.png");

                else if (count == 4)
                    overlay.loadFromFile("../images/number_4.png");

                else if (count == 5)
                    overlay.loadFromFile("../images/number_5.png");

                else if (count == 6)
                    overlay.loadFromFile("../images/number_6.png");

                else if (count == 7)
                    overlay.loadFromFile("../images/number_7.png");

                else if (count == 8)
                    overlay.loadFromFile("../images/number_8.png");

                if (count != 0)
                    temp.update(overlay);
                else
                    revealNeighbors();
                tilePic.setTexture(temp);
            }
        }
}
bool Tile::getBomb(){
    return Bomb;
}

void Tile::setBomb(bool test){
    Bomb = test;
}

// tile neighbors getter and setters
std::array<Tile*, 8>& Tile::getNeighbors(){
    return neighbors;
}

// Click actions
void Tile::onClickLeft()
{
    if (getState() == HIDDEN)
        setState(REVEALED);

}
void Tile::onClickRight()
{
    if (getState() == HIDDEN)
        setState(FLAGGED);
    else if(getState() == FLAGGED)
        setState(HIDDEN);
}

void Tile::draw(){

}

void Tile::revealNeighbors()
{
    for (int i = 0; i < 8; i++)
    {
        if (neighbors[i] != nullptr) {
            if (neighbors[i]->getState() != REVEALED && neighbors[i]->getState() != FLAGGED && !neighbors[i]->getBomb())
                neighbors[i]->setState(REVEALED);
        }
    }
}

