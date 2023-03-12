#include "GameWindow.h"

void GameWindow::update() {
    const int size = static_cast<int> (std::min(getSize().x, getSize().y));
    const sf::Vector2f cellSize = sf::Vector2f(size / GRID_SIZE_X, size / GRID_SIZE_Y);
    sf::Vector2f topLeft = sf::Vector2f((getSize().x - size) / 2, (getSize().y - size) / 2);

    sf::Event event;
    while (pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                close();
                break;

            case sf::Event::Resized: {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                setView(sf::View(visibleArea));
            }
                break;

            case sf::Event::MouseButtonPressed:

                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (game.winner != Players::neutral) {
                        game.init();
                    } else if (game.curPlayer == Players::blue) {
                        game.update((event.mouseButton.x - topLeft.x) / cellSize.x);
                    } else {
                        Move move = ai.findBestMove();
                        game.update(move.row);
                    }
                    ai.update(game);
                }
        }
    }
}

void GameWindow::render() {

    clear(sf::Color::White);

    const int size = static_cast<int> (std::min(getSize().x, getSize().y));
    const sf::Vector2f cellSize = sf::Vector2f(size / GRID_SIZE_X, size / GRID_SIZE_Y);
    sf::Vector2f topLeft = sf::Vector2f((getSize().x - size) / 2, (getSize().y - size) / 2);

    // draw field
    sf::CircleShape circ;
    sf::RectangleShape rect;
    rect.setOutlineColor(sf::Color::Black);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineThickness(2);
    rect.setSize(cellSize);
    rect.setOrigin(rect.getGlobalBounds().width / 2, rect.getGlobalBounds().height / 2);
    circ.setRadius(std::min(cellSize.x, cellSize.y) / 2 - 4);
    circ.setOrigin(circ.getGlobalBounds().width / 2, circ.getGlobalBounds().height / 2);
    circ.setOutlineThickness(1);
    for (int x = 0; x < GRID_SIZE_X; ++x) {
        for (int y = 0; y < GRID_SIZE_Y; ++y) {

            // draw outer rect

            rect.setPosition(topLeft.x + x * cellSize.x + cellSize.x / 2,
                             topLeft.y + (GRID_SIZE_Y - y - 1) * cellSize.y + cellSize.x / 2);
            draw(rect);

            // draw player mark
            if (game.grid[x][y] != Players::neutral) {

                circ.setPosition(rect.getPosition());
                switch (game.grid[x][y]) {
                    case Players::blue:
                        circ.setFillColor(sf::Color(0, 0, 255, 50));
                        circ.setOutlineColor(sf::Color(0, 0, 255, 200));
                        break;
                    case Players::red:
                        circ.setFillColor(sf::Color(255, 0, 0, 50));
                        circ.setOutlineColor(sf::Color(255, 0, 0, 200));
                        break;
                    default:
                        break;
                }
                draw(circ);
            }
        }
    }

    // Draw winner text
    if (game.winner != Players::neutral) {
        sf::Text text;
        text.setFont(gameFont);
        text.setCharacterSize(50);
        text.setString("Spieler " + std::to_string(game.winner) + " hat gewonnen!");
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(2);
        if ((frames / 5) % 2) {
            text.setFillColor(sf::Color::Black);
        } else {
            if (game.winner == Players::blue)
                text.setFillColor(sf::Color::Blue);
            else
                text.setFillColor(sf::Color::Red);
        }
        text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
        text.setPosition(topLeft.x + size / 2, topLeft.y + size / 2);
        draw(text);
    }

    frames++;
    display();
}

GameWindow::GameWindow(sf::VideoMode
                       mode,
                       const sf::String &title, sf::Uint32
                       style,
                       const sf::ContextSettings &settings) : RenderWindow(mode, title, style, settings) {
    if (!gameFont.loadFromFile("fonts/arial.ttf")) {
        close();
    }
}



