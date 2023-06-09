#ifndef CONNECT4_GAMEWINDOW_H
#define CONNECT4_GAMEWINDOW_H

#include <SFML/Graphics.hpp>
#include "GameLogic.h"
#include "AI.h"

class GameWindow : public sf::RenderWindow {

public:

    sf::Font gameFont;

    GameLogic game = GameLogic();
    AI ai = AI();

    GameWindow() = default;
    GameWindow(sf::VideoMode mode, const sf::String& title, sf::Uint32 style = sf::Style::Default, const sf::ContextSettings& settings = sf::ContextSettings());

    void update();
    void render();
    int frames = 0;
};


#endif //CONNECT4_GAMEWINDOW_H
