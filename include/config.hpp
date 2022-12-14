#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

namespace cfg {

// Length & Width of each square cell within the board
static const float BLOCK_SIZE = 40.0f;

/* Game Difficulty Level */
enum GameLevel {
    EASY,
    MEDIUM,
    HARD
};

class ArgParser {
    GameLevel defaultLevel() const;

   public:
    /* Reads the game level from a string */
    GameLevel readArgs(int argc, char* argv[]) const;
};

struct GameConfig {
    unsigned int numCols;
    unsigned int numRows;
    unsigned int numMines;
    const sf::Font* font;
    GameConfig(const GameLevel level, const sf::Font& font);
    unsigned int numRemaining() const;
    // Global window size in pixels
    float windowHeight() const;
    float windowWidth() const;
    // Converts a global xy-value to a board-cell position
    unsigned int xToCoord(int x) const;
    unsigned int yToCoord(int y) const;
    // Checks whether the xy-coordinate exists within the board
    bool exists(unsigned int x, unsigned int y) const;
};

}  // namespace cfg

#endif
