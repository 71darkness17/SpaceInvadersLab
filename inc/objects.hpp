#ifndef OBJECTS_HPP
#define OBJECTS_HPP
#include <iostream>
#include <cstdint>
#include <string>
#include <cmath>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#define BUTTON_TEXT_SIZE 32
// class Object {
// private:
//     float x,y;
// public:
//     Object(float posx, float posy);

// };

enum class EntityType{
    Player, Enemy, Static
};


class Object{
protected:
    sf::Texture texture;
    sf::Sprite sprite;
public:
    Object(float posx, float posy, const std::string &pathToTexture);
    const bool isCollide(const float &x, const float &y) const;
    const bool isCollide(const Object &other) const;
    void move(float d_x, float d_y);
    const sf::Sprite getSprite() const;
    const sf::Rect<float> getRect() const;
    const sf::Vector2f& getPosition() const;
    void setPosition(float x, float y);
    void setRotation(float angle);
    ~Object() = default;
};

class Bullet: public Object{
private:
    float d_x,d_y;
    EntityType owner;
public:
    Bullet(float posx,float posy, const std::string &pathToTexture,float dd_x,float dd_y, EntityType type);
    const EntityType getType() const; 
    void move();
};

class Ship: public Object{
protected:
    std::string bulletPath;
public:
    Ship(float posx,float posy, const std::string &pathToTexture, const std::string &pathToBulletTexture);
    virtual Bullet* shoot() const = 0;
    ~Ship() = default;
};

class Player: public Ship{
private:
    int hp;
public:
    sf::Time lastShot;
    Player(float posx, float posy, const std::string &pathToTexture, int startHP, const std::string &pathToBulletTexture);
    Bullet* shoot() const override;
    void damage();
    const bool isAlive() const;
    void setHealth(int newHp);
};

class Enemy: public Ship{
public:
    Enemy(float posx, float posy, const std::string &pathToTexture, const std::string &pathToBulletTexture);
    Bullet* shoot() const override;
    Bullet* shoot(float x, float y) const;
    Bullet* shoot(sf::Vector2f vec) const;
};

class Button: public Object{
private:
    sf::Text text;
    sf::Texture hoverTexture;
    int* var;
    int sw;
public:
    Button(float posx,float posy,const sf::Font &font,const std::string &buttonText,const std::string &texturePath,const std::string &hoverPath);
    void setAction(int* newVar, int newSw);
    void hover();
    void unhover();
    void click();
    static void defaultAction(int temp) {};
    void setPosition(float x,float y);
    const sf::Text& getText() const; 
};



#endif