#include <SFML/Graphics.hpp>
#include <ctime>

#include "config.hpp"
#include "game.hpp"

int main(int argc, char* argv[]) {
    std::srand(std::time(nullptr));
    cfg::GameLevel level = cfg::ArgParser().readArgs(argc, argv);
    sf::Font font;
    if (!font.loadFromFile("font.ttf")) {
        std::cerr << "Could not find font file!" << std::endl;
        return -1;
    }
    cfg::GameConfig gameConfig(level, font);
    core::Board board(gameConfig);
    sf::RenderWindow window(sf::VideoMode(gameConfig.windowWidth(), gameConfig.windowHeight()), "Minesweeper");
    window.setFramerateLimit(20);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                // Shutdown game
                window.close();
            } else if (!board.isGameover() && event.type == sf::Event::MouseButtonReleased) {
                sf::Event::MouseButtonEvent ev = event.mouseButton;
                unsigned int xPos = gameConfig.xToCoord(ev.x);
                unsigned int yPos = gameConfig.yToCoord(ev.y);
                // Left or Right button illicits a reaction
                if (ev.button == sf::Mouse::Left) {
                    board.selectCell(xPos, yPos);
                } else if (ev.button == sf::Mouse::Right) {
                    board.flagCell(xPos, yPos);
                }
                // Otherwise, do nothing
            }
        }

        window.clear();
        board.render(window);
        window.display();
    }

    return 0;
}
