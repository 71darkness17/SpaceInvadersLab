#include"objects.hpp"

// //Object interface
// Object::Object(float posx, float posy){
//     x = posx;
//     y = posy;
// }

//Object interface
Object::Object(float posx, float posy, const std::string &pathToTexture){
    if(!texture.loadFromFile(pathToTexture)) throw "Cannot load texture";
    sprite.setTexture(texture);
    sprite.setPosition(posx,posy);
}
const bool Object::isCollide(const float &x, const float &y) const{
    if(sprite.getGlobalBounds().contains(x,y)) return true;
    return false;
}
const bool Object::isCollide(const Object &other) const {
    if(sprite.getGlobalBounds().intersects(other.getRect())) return true;
    return false;
}
void Object::move(float d_x, float d_y){
    sprite.move(d_x,d_y);
}
const sf::Sprite Object::getSprite() const{
    return sprite;
}
const sf::Rect<float> Object::getRect() const{
    return sprite.getGlobalBounds();
}
const sf::Vector2f& Object::getPosition() const{
    return sprite.getPosition();
}
void Object::setPosition(float x, float y){
    sprite.setPosition(x,y);
}
void Object::setRotation(float angle){
    sprite.setRotation(angle);
}

//Ship Interface
Ship::Ship(float posx,float posy, const std::string &pathToTexture, const std::string &pathToBulletTexture):\
 Object(posx,posy,pathToTexture), bulletPath(pathToBulletTexture) {}

//Player interface
Player::Player(float posx, float posy, const std::string &pathToTexture, int startHP, const std::string &pathToBulletTexture):\
 Ship(posx,posy,pathToTexture,pathToBulletTexture), hp(startHP), lastShot(sf::Time::Zero) {}
Bullet* Player::shoot() const{
    Bullet* bul = new Bullet(getPosition().x,getPosition().y,bulletPath,0,-2,EntityType::Player);
    return bul;
}
void Player::damage(){
    if(hp!=0) hp--;
}
const bool Player::isAlive() const{
    if(hp!=0) return true;
    return false;
}
void Player::setHealth(int newHp) {
    if(newHp>0) hp = newHp;
    else throw "Invalid health points";
}

//Enemy interface
Enemy::Enemy(float posx,float posy, const std::string &pathToTexture, const std::string &pathToBulletTexture):\
 Ship(posx, posy,pathToTexture,pathToBulletTexture){}
Bullet* Enemy::shoot() const {
    Bullet* bul = new Bullet(getPosition().x,getPosition().y,bulletPath,0,1,EntityType::Enemy);
    return bul;
}
Bullet* Enemy::shoot(float x, float y) const{
    sf::Vector2f ort;
    ort.x = x - getPosition().x;
    ort.y = y - getPosition().y;
    float length = std::sqrt(ort.x*ort.x+ort.y+ort.y);
    float angle = std::acos(ort.y/length);
    ort.x = ort.x / length;
    ort.y = ort.y / length;
    Bullet* bul = new Bullet(getPosition().x,getPosition().y,bulletPath,ort.x,ort.y,EntityType::Enemy);
    bul->setRotation(angle);
    return bul;
}
Bullet* Enemy::shoot(sf::Vector2f vec) const{
    return shoot(vec.x,vec.y);
}

//Bullet interface
Bullet::Bullet(float posx, float posy, const std::string &pathToTexture, float dd_x, float dd_y, EntityType type):\
 Object(posx,posy,pathToTexture), d_x(dd_x), d_y(dd_y), owner(type) {}
void Bullet::move() {
    Object::move(d_x,d_y);
}
const EntityType Bullet::getType() const{
    return owner;
}

//Button interface

Button::Button(float posx,float posy,const sf::Font &font,const std::string &buttonText,const std::string &texturePath,const std::string &hoverPath):\
 Object(posx,posy,texturePath){
    text = sf::Text(buttonText,font,BUTTON_TEXT_SIZE);
    text.setPosition(sprite.getPosition());
    if(!hoverTexture.loadFromFile(hoverPath)) throw "Cannot load hover texture";
}
void Button::setAction(int* newVar, int newSw){
    var = newVar;
    sw = newSw;
}
void Button::hover(){
    sprite.setTexture(hoverTexture);
}
void Button::unhover(){
    sprite.setTexture(texture);
}
void Button::click(){
    *var = sw;
}
void Button::setPosition(float x,float y){
    Object::setPosition(x,y);
    float center_x = sprite.getPosition().x + sprite.getGlobalBounds().width/2.0f;
    float center_y = sprite.getPosition().y + sprite.getGlobalBounds().height/2.0f;
    text.setPosition(center_x-text.getGlobalBounds().width/2.0f,center_y-text.getGlobalBounds().height/2.0f);
}
const sf::Text& Button::getText() const{
    return text;
}
