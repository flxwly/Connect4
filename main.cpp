#include <iostream>
#include "GameWindow.h"

int main() {

    GameWindow window = GameWindow(sf::VideoMode(800, 800), "Connect4");
    window.setFramerateLimit(30);

    while (window.isOpen()) {
        window.update();
        window.render();
    }

    return 0;
}
