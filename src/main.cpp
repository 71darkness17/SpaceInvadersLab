#include "objects.hpp"
#include"game.hpp"

int main(){
    sf::Font font;
    if(!font.loadFromFile("../font.ttf")) throw "Cannot load font";
    std::cout<<"font loaded"<<std::endl;
    Game game(Mode::Main,Difficult::Medium,font,"SpaceIvaders");
    game.gameloop();
    return 0;
}
