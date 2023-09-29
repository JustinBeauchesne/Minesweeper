#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

// i didnt use this class didnt understand the callBack thing
// Prototypes
class Button
{
    sf::Sprite* buttonPic;
public:
    Button(sf::Vector2f _position, std::function<void(void)> _onClick);

    sf::Vector2f getPosition();

    sf::Sprite* getSprite();

    void setSprite(sf::Sprite* _sprite);

    void onClick();
};

// Definitions
Button::Button(sf::Vector2f _position, std::function<void(void)> _onClick){
    buttonPic = new sf::Sprite;
    buttonPic->setPosition(_position);
}

sf::Vector2f Button::getPosition(){
    return buttonPic->getPosition();
}

sf::Sprite* Button::getSprite(){
    return buttonPic;
}

void Button::setSprite(sf::Sprite* _sprite){
    buttonPic = _sprite;
}

void onClick(){
    sf::Event press;
    if (press.type == sf::Event::MouseButtonPressed){
        if(press.mouseButton.button == sf::Mouse::Right){
            //Tile::onClickRight();
        }
    }
}

