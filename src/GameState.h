#pragma once
#include <SFML/Graphics.hpp>
#include "Tile.h"
#include <fstream>
#include <algorithm>

enum PlayStatus { WIN, LOSS, PLAYING };

class GameState
{
    // variables
    friend class Tile;
    int mineCount = 0;
    PlayStatus status;
    int flagCount = 0;

public:
    std::map<std::pair<int,int>, Tile*> toSearch;

// prototypes
    GameState(sf::Vector2i _dimensions = sf::Vector2i(25, 16), int _numberOfMines = 50);
    GameState(const char* filepath);
    ~GameState();

    // random assign mine to tile without duplicate
    void randomBoard();
    int getFlagCount();
    void setFlagCount(int num);
    int getMineCount();

    // iterate through board to find matching coords
    Tile* getTile(int x, int y);

    PlayStatus getPlayStatus();
    PlayStatus setPLayStatus(PlayStatus _status);

    void makeBoard();
    void addSpecial();
    void displayFlags();

    // moved Set neighbors here to make use of game board
    // set every neighbor all at once rather than each tile individually
    void setNeighbors();
};

// define
GameState::GameState(sf::Vector2i _dimensions , int _numberOfMines){
    // initialize new board, play status and tile neighbors
    status = PLAYING;
    makeBoard();
    setNeighbors();
    addSpecial();
    mineCount = _numberOfMines;
    randomBoard();
}

GameState::GameState(const char* filepath){
    // reset board, play status
    status = PLAYING;
    // clear prev board
    toSearch.clear();
    makeBoard();
    setNeighbors();
    addSpecial();

    mineCount = 0; // reset mineCount just in case
    // read in stream to change tile isBomb condition
    std::ifstream inStream(filepath);
    for(int row = 0; row < 16; row++){
        std::string str;
        std::getline(inStream, str);
        for(int col = 0; col < str.length(); col++){
            char num = str.at(col);
            if(num == '1'){
                Tile* temp = toSearch[std::make_pair(col,row)];
                temp->setBomb(true);
                mineCount++;
            }
        }
    }

}

GameState::~GameState(){
    for(auto iter = this->toSearch.begin(); iter != this->toSearch.end(); iter++){
        delete iter->second;
    }
    toSearch.clear();
}

// random assign mine to tile without duplicate
void GameState::randomBoard(){
    for(int i = 0; i< mineCount; i++){
        std::map<std::pair<int,int>, bool> coords;
        int xCord  = rand() % 25;
        int yCord = rand() % 16;
        // search idea from https://www.geeksforgeeks.org/map-find-function-in-c-stl/
        if(coords.find(std::make_pair(xCord,yCord)) != coords.end()){
            i--;
        }
        else{
            coords[std::make_pair(xCord,yCord)] = true;
            getTile(xCord,yCord)->setBomb(true);
        }
    }
}
int GameState::getFlagCount(){
    return flagCount;
}
void GameState::setFlagCount(int num){
    flagCount = num;
}
int GameState::getMineCount(){
    return mineCount;
}

// iterate through board to find matching coords
Tile* GameState::getTile(int x, int y){
    try{
        return toSearch.at(std::make_pair(x,y));
    }
    catch(const std::out_of_range& oor)
    {
        return nullptr;
    }
}

PlayStatus GameState::getPlayStatus(){
    return status;
}
PlayStatus GameState::setPLayStatus(PlayStatus _status){
    status = _status;
    Tile* currTile = getTile(11.5,16);
    if(getPlayStatus() == PLAYING){
        currTile->temp.loadFromFile("../images/face_happy.png");
        currTile->tilePic.setTexture(currTile->temp);
    }
    else if(getPlayStatus() == WIN){
        currTile->temp.loadFromFile("../images/face_win.png");
        currTile->tilePic.setTexture(currTile->temp);
    }
    else if(getPlayStatus() == LOSS){
        currTile->temp.loadFromFile("../images/face_lose.png");
        currTile->tilePic.setTexture(currTile->temp);
    }
}

void GameState::makeBoard(){
    Tile* newTile;
    for(int row = 0; row < 16; row++){
        for(int col = 0; col < 25; col++)
        {
            std::pair point (col,row);
            newTile = new Tile(sf::Vector2f(col,row));
            newTile->setState(HIDDEN);
            toSearch[point] = newTile;
        }
    }
}

void GameState::addSpecial(){
    // smile face tile
    Tile* newTile;
    newTile = new Tile(sf::Vector2f(11.5,16));
    newTile->temp.loadFromFile("../images/face_happy.png");
    newTile->tilePic.setTexture(newTile->temp, true);
    toSearch[std::make_pair(11.5,16)] = newTile;
    //debug tile
    newTile = new Tile(sf::Vector2f(16,16));
    newTile->temp.loadFromFile("../images/debug.png");
    newTile->tilePic.setTexture(newTile->temp,true);
    toSearch[std::make_pair(15.5,16)] = newTile;
    // test 1 tile
    newTile = new Tile(sf::Vector2f(18,16));
    newTile->temp.loadFromFile("../images/test_1.png");
    newTile->tilePic.setTexture(newTile->temp,true);
    toSearch[std::make_pair(18.5,16)] = newTile;
    // test 2 tile
    newTile = new Tile(sf::Vector2f(20,16));
    newTile->temp.loadFromFile("../images/test_2.png");
    newTile->tilePic.setTexture(newTile->temp,true);
    toSearch[std::make_pair(21.5,16)] = newTile;
    // test 2 tile
    newTile = new Tile(sf::Vector2f(22,16));
    newTile->temp.loadFromFile("../images/Test_3.png");
    newTile->tilePic.setTexture(newTile->temp,true);
    toSearch[std::make_pair(24.5,16)] = newTile;
    // flag tiles dig 1
    newTile = new Tile(sf::Vector2f(0,16));
    newTile->temp.loadFromFile("../images/digits.png",sf::Rect(0,0,21,32));
    newTile->tilePic.setTexture(newTile->temp,true);
    newTile->currentState = FLAGGED;
    toSearch[std::make_pair(0,16)] = newTile;
    // flag tiles dig 2
    newTile = new Tile(sf::Vector2f(float(21)/32,16));
    newTile->temp.loadFromFile("../images/digits.png",sf::Rect(21*5,0,21,32));
    newTile->tilePic.setTexture(newTile->temp,true);
    newTile->currentState = FLAGGED;
    toSearch[std::make_pair(21/32,16)] = newTile;
    // flag tiles dig 3
    newTile = new Tile(sf::Vector2f(float(42)/32,16));
    newTile->temp.loadFromFile("../images/digits.png",sf::Rect(0,0,21,32));
    newTile->tilePic.setTexture(newTile->temp,true);
    newTile->currentState = FLAGGED;
    toSearch[std::make_pair(42/32,16)] = newTile;
}
void GameState::displayFlags(){
    int flagger = getMineCount() - getFlagCount();
    Tile* curr;
    if(flagger < 0){
        // set negative sign
        curr = getTile(0,16);
        curr->temp.loadFromFile("../images/digits.png",sf::Rect(21*10,0,21,32));
        curr->tilePic.setTexture(curr->temp);
    }
    else{
        // set zero for 1st dig
        curr = getTile(0,16);
        curr->temp.loadFromFile("../images/digits.png",sf::Rect(0,0,21,32));
        curr->tilePic.setTexture(curr->temp);
    }
    for(int i = 0; i < 10;i++){
        if(flagger/10 == i){
            // set 2nd dig *21 pixels
            curr = getTile(float(21)/32,16);
            curr->temp.loadFromFile("../images/digits.png",sf::Rect(21*i,0,21,32));
            curr->tilePic.setTexture(curr->temp);
        }
        if(flagger % 10 == i){
            // set 3rd dig *21 pixels
            curr = getTile(float(42)/32,16);
            curr->temp.loadFromFile("../images/digits.png",sf::Rect(21*i,0,21,32));
            curr->tilePic.setTexture(curr->temp);
        }
    }
}


// moved Set neighbors here to make use of game board
// set every neighbor all at once rather than each tile individually
void GameState::setNeighbors()
{
    for (auto iter = toSearch.begin(); iter != toSearch.end(); iter++) {
        int x = iter->second->getLocation().x / 32;
        int y = iter->second->getLocation().y / 32;
        iter->second->neighbors[0] = getTile(x - 1, y - 1);
        iter->second->neighbors[1] = getTile(x, y - 1);
        iter->second->neighbors[2] = getTile(x + 1, y - 1);
        iter->second->neighbors[3] = getTile(x - 1, y);
        iter->second->neighbors[4] = getTile(x + 1, y);
        iter->second->neighbors[5] = getTile(x - 1, y + 1);
        iter->second->neighbors[6] = getTile(x, y + 1);
        iter->second->neighbors[7] = getTile(x + 1, y + 1);
    }
}
