#include"game.hpp"

//Page interface
Page::Page(const std::vector<Button*> buttonsVec): buttons(buttonsVec) {}
Page::~Page(){
    for(size_t i = 0;i!=buttons.size();++i){
        delete buttons[i];
    }
}
void Page::makeAction(float x, float y){
    for(size_t i = 0;i!=buttons.size();++i){
        if(buttons[i]->isCollide(x,y)) buttons[i]->click();
    }
}
void Page::makeAction(sf::Vector2f vec){
    makeAction(vec.x,vec.y);
}
void Page::hover(float x, float y){
    for(size_t i = 0;i!=buttons.size();++i){
        if(buttons[i]->isCollide(x,y)) buttons[i]->hover();
        else buttons[i]->unhover();
    }
}
void Page::hover(sf::Vector2f vec){
    hover(vec.x,vec.y);
}
void Page::draw(sf::RenderWindow &window){
    float i = (float) window.getSize().y/2;
    for(Button* button: buttons){
        if(button->getPosition().x >= 0.0f){
            button->setPosition(button->getPosition().x,i);
            i+= button->getRect().height+5;
        }
        else if(button->getPosition().y >= 0.0f){
            button->setPosition((float) window.getSize().x/2 - button->getRect().width/2,button->getPosition().y);
        }
        else{
            button->setPosition((float) window.getSize().x/2 - button->getRect().width/2,i);
            i+= button->getRect().height+5;
        }
        window.draw(button->getSprite());
        window.draw(button->getText());
    }
}

//Game realization
Game::Game(Mode startMode, Difficult startDifficult,const sf::Font &font, std::string title):\
 currentMode(startMode), currentDifficult(startDifficult), window(sf::VideoMode(1920,1080),title,sf::Style::Fullscreen), header(title,font,64U), clock(sf::Clock()), enemyDirection(1), lastDowngoing(sf::Time::Zero)
{
    window.setFramerateLimit(60);
    enemiesToDelete.resize(3);
    header.setColor(sf::Color::Green);
    float header_x = window.getSize().x/2-header.getGlobalBounds().width/2.0f;
    float header_y = window.getSize().y/3;
    header.setPosition(header_x,header_y);
    std::cout<<"Start game constructor"<<std::endl;
    //Creating player
    player  = new Player(window.getSize().x/2,window.getSize().y*4/5,PATH_TO_PLAYER,3,PATH_TO_PBULLET);
    std::cout<<"Player created"<<std::endl;
    //Creating enemies
    enemies.resize(3,std::list<Enemy*>(0));
    for(int j = 0;j<enemies.size();++j){
        for(int i = 16;i<16+11*(80);i+=80){
            enemies[j].push_back(new Enemy(i,(1+j)*window.getSize().y/6,PATH_TO_ENEMY1,PATH_TO_EBULLET1));
        }
    }   
    std::cout<<"Enemies created"<<std::endl;
    
    //Main page buttons
    std::vector<Button*> p0;
    p0.push_back(new Button(-1,-1,font,"Play",PATH_TO_BUTTON,PATH_TO_HBUTTON));
    p0.push_back(new Button(-1,-1,font,"Difficult",PATH_TO_BUTTON,PATH_TO_HBUTTON));
    p0.push_back(new Button(-1,-1,font,"Quit",PATH_TO_BUTTON,PATH_TO_HBUTTON));
    p0[0]->setAction((int*) &currentMode,(int)Mode::Play);
    p0[1]->setAction((int*) &currentMode,(int)Mode::Difficult);
    p0[2]->setAction((int*) &currentMode,(int)Mode::Quit);

    //Difficult page button
    std::vector<Button*> p1;
    p1.push_back(new Button(-1,-1,font,"Easy",PATH_TO_BUTTON,PATH_TO_HBUTTON));
    p1.push_back(new Button(-1,-1,font,"Medium",PATH_TO_BUTTON,PATH_TO_HBUTTON));
    p1.push_back(new Button(-1,-1,font,"Insane",PATH_TO_BUTTON,PATH_TO_HBUTTON));
    p1.push_back(new Button(-1,-1,font,"Main Menu",PATH_TO_BUTTON,PATH_TO_HBUTTON));
    p1[0]->setAction((int*) &currentDifficult,(int) Difficult::Easy);
    p1[1]->setAction((int*) &currentDifficult,(int) Difficult::Medium);
    p1[2]->setAction((int*) &currentDifficult,(int) Difficult::Insane);
    p1[3]->setAction((int*) &currentMode,(int) Mode::Main);
    std::cout<<"Buttons created"<<std::endl;
    //Creating pages
    pages.push_back(new Page(p0));
    pages.push_back(new Page(p1));
    std::cout<<"Pages created/Game constructor ended"<<std::endl;
}
Game::~Game(){
    for(auto page: pages){
        delete page;
    }
    for(auto &j: enemies){
        for(Enemy* enemy: j){
            delete enemy;
        }
        j.clear();
    }
    for(auto bullet: bullets){
        delete bullet;
    }
    delete player;
}
void Game::gameloop(){
    clock.restart();
    while(window.isOpen()){
        window.clear();
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed) {
                window.close();
                return;
            }
        }
        //std::cout<<"Event checked"<<std::endl;
        if(currentMode == Mode::Play) {
            std::cout<<"rendering game"<<std::endl;
            renderGame();
        }
        else if(currentMode == Mode::Quit) {
            window.close();
            return;
        }
        else renderPage();
        //std::cout<<"Mode choosed"<<std::endl;
        window.display();
    }
}
void Game::renderPage(){
    window.setMouseCursorVisible(true);
    sf::Vector2f mouse_pos;
    mouse_pos.x = (float) sf::Mouse::getPosition().x - window.getPosition().x;
    mouse_pos.y = (float) sf::Mouse::getPosition().y - window.getPosition().y;
    std::cout<<"Position calculated"<<std::endl;
    pages[(int) currentMode]->hover(mouse_pos);
    if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
        pages[(int) currentMode]->makeAction(mouse_pos);
    }
    std::cout<<"Action made"<<std::endl;
    window.draw(header);
    if((int) currentMode<2) pages[(int) currentMode]->draw(window);
    else if(currentMode == Mode::Play) restartGame();
    std::cout<<"Page "<<(int) currentMode<<" drawn"<<std::endl;
}

void Game::renderGame(){
    window.setMouseCursorVisible(false);
    //player controls
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && (int) player->getPosition().x > 0) player->move(-3,0);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) && player->getPosition().x - window.getSize().x-player->getRect().width < 0.001f) player->move(3,0);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
        if(clock.getElapsedTime().asSeconds() - player->lastShot.asSeconds() >= 1.0f){
            bullets.push_back(player->shoot());
            player->lastShot = clock.getElapsedTime();
        }
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        currentMode = Mode::Main;
        return;
    }
    float d_y = -1;
    for(auto &j: enemies){
        if((int) j.front()->getPosition().x <= 0) enemyDirection = 1.0f;
        else if (j.back()->getPosition().x - window.getSize().x >= 0.001f) enemyDirection = -1.0f;
        
    }
    for(auto &j:enemies){
        if(!j.empty() && clock.getElapsedTime().asSeconds()-lastDowngoing.asSeconds()>10.0f){
            d_y = j.front()->getRect().height+5;
            lastDowngoing = clock.getElapsedTime();
            break;
        }
    }
    if(d_y == -1) d_y = 0;
    for(auto &j: enemies){
        for(auto enemy: j){
            enemy->move(enemyDirection,d_y);
            if(rand()%1000 == 0) {
                if(currentDifficult == Difficult::Insane) bullets.push_back(enemy->shoot(player->getPosition()));
                else bullets.push_back(enemy->shoot());
            }
        }}
    //bullets control
    if(!bullets.empty()){
        for(auto bullet = bullets.begin();bullet!=bullets.end();++bullet){
            (*bullet)->move();
            if((*bullet)->getType() == EntityType::Enemy){
                if(((Bullet*)*bullet)->isCollide(*player)) {
                    player->damage();
                    bulletsToDelete.push_back(bullet);
                }
            }
            else if((*bullet)->getType() == EntityType::Player){
                for(int i = 0;i<enemies.size();++i){
                    for(auto enemy = enemies[i].begin(); enemy!= enemies[i].end(); ++enemy){
                        if((*bullet)->isCollide(**enemy)) {
                            if(enemiesToDelete[i].empty() || enemiesToDelete[i].back() != enemy){
                                enemiesToDelete[i].push_back(enemy);
                            }
                            if(bulletsToDelete.empty() || bulletsToDelete.back() != bullet){
                                bulletsToDelete.push_back(bullet);
                            }
                            break;
                        }
                    }}
            }
            sf::Vector2f bullet_pos = (*bullet)->getPosition();
            if(bullet_pos.x>window.getSize().x || bullet_pos.x<0 || bullet_pos.y>window.getSize().y || bullet_pos.y<0){
                if(!bulletsToDelete.empty() && bulletsToDelete.back() != bullet){
                    bulletsToDelete.push_back(bullet);
                }
            }
            std::cout<<"start deleting enemies "<<enemiesToDelete.size()<<std::endl;
            int j = 0;
            for(int k = 0;k<enemiesToDelete.size();++k){
                for(auto i: enemiesToDelete[k]){
                    std::cout<<"deleting "<<enemiesToDelete[k].size()<<std::endl;
                    delete *i;
                    std::cout<<"erasing "<<j<<std::endl;
                    enemies[k].erase(i);
                    ++j;
                }
            }
            for(auto &i: enemiesToDelete){
                i.clear();
            }
            std::cout<<"deleted all enemies"<<std::endl;
        }
        for(auto i: bulletsToDelete){
            delete *i;
            bullets.erase(i);
        }
        std::cout<<"deleted all bullets"<<std::endl;
        
        bulletsToDelete.clear();
        
    }
    //check if player died or if all enemies died
    if(!player->isAlive() || enemies[0].empty() || enemies[1].empty() || enemies[2].empty()) {
        currentMode = Mode::Main;
        return;
    }
    //check if enemies got to player
    for(auto &j: enemies){
        if(!j.empty() && j.front()->getPosition().y >= player->getPosition().y){
            currentMode = Mode::Main;
            return;
        }
    }
    //draw all objects
    window.draw(player->getSprite());
    for(auto &j: enemies){
        for(auto enemy: j){
            window.draw(enemy->getSprite());
        }
    }
    for(auto bullet: bullets){
        window.draw(bullet->getSprite());
    }
}
void Game::restartGame(){
    //delete bullets
    for(Bullet *bullet: bullets){
        delete bullet;
    }
    bullets.clear();
    //delete enemies
    for(auto &j: enemies){
        for(Enemy* enemy: j){
            delete enemy;
        }
        j.clear();
    }
    enemies.clear();
    //remake enemies
    enemies.resize(3,std::list<Enemy*>(0));
    for(int j = 0;j<enemies.size();++j){
        for(int i = 16;i<16+11*(80);i+=80){
            enemies[j].push_back(new Enemy(i,(1+j)*window.getSize().y/6,PATH_TO_ENEMY1,PATH_TO_EBULLET1));
        }
    }
    std::cout<<"Enemies created"<<std::endl;
    //restore player
    player->setHealth(3);
    player->setPosition(window.getSize().x/2,window.getSize().y*4/5);
}