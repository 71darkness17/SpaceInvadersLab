#ifndef GAME_HPP
#define GAME_HPP

#include<iostream>
#include<random>
#include<list>
#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Audio.hpp>
#include"objects.hpp"
#include<set>

#define PATH_TO_PLAYER "../player.png"
#define PATH_TO_PBULLET "../bullet.png"
#define PATH_TO_ENEMY1 "../enemy1.png"
#define PATH_TO_EBULLET1 "../bullet.png"
#define PATH_TO_BUTTON "../buttonWide.png"
#define PATH_TO_HBUTTON "../buttonWide.png"
#define SHOOT_COOLDOWN 1000

enum class Mode{
    Main, Difficult, Quit, Play
};

enum class Difficult{
    Easy, Medium, Insane
};

class Page{
private:
    std::vector<Button*> buttons;
public:
    Page(const std::vector<Button*> buttonsVec);
    void makeAction(float posx, float posy);
    void makeAction(sf::Vector2f vec);
    void hover(float posx, float posy);
    void hover(sf::Vector2f vec);
    void draw(sf::RenderWindow &window);
    ~Page();
};

class Game{
private:
    std::vector<Page*> pages;
    Player* player;
    std::vector<std::list<Enemy*>> enemies;
    std::list<Bullet*> bullets;
    Mode currentMode;
    Difficult currentDifficult;
    sf::RenderWindow window;
    sf::Text header;
    sf::Clock clock;
    float enemyDirection;
    sf::Time lastDowngoing;
    std::vector<std::vector<std::list<Enemy*>::iterator>> enemiesToDelete;
    std::vector<std::list<Bullet*>::iterator>  bulletsToDelete;
public:
    Game(Mode startMode, Difficult startDifficult,const sf::Font &font, std::string title);
    void renderGame();
    void renderPage();
    void gameloop();
    void restartGame();
    ~Game();
};

#endif