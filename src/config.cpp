#include "config.hpp"

namespace cfg {

GameLevel ArgParser::defaultLevel() const {
    return GameLevel::EASY;
}

GameLevel ArgParser::readArgs(int argc, char* argv[]) const {
    if (argc < 2) {
        std::cerr << "Using default difficulty level." << std::endl;
        return this->defaultLevel();
    } else {
        const std::string level = argv[1];
        if (argc > 2) {
            std::cerr << "Only using first argument for input parameter (" << level << ")." << std::endl;
        }
        if (level == "easy") {
            return GameLevel::EASY;
        } else if (level == "medium") {
            return GameLevel::MEDIUM;
        } else if (level == "hard") {
            return GameLevel::HARD;
        } else {
            std::cerr << "Unrecognized difficulty level value! Using default." << std::endl;
            return this->defaultLevel();
        }
    }
}

GameConfig::GameConfig(const GameLevel level, const sf::Font& font) {
    this->font = &font;
    switch (level) {
        case GameLevel::EASY:
            numCols = 8;
            numRows = 8;
            numMines = 10;
            break;
        case GameLevel::MEDIUM:
            numCols = 16;
            numRows = 16;
            numMines = 40;
            break;
        case GameLevel::HARD:
            numCols = 30;
            numRows = 16;
            numMines = 99;
            break;
    }
}

unsigned int GameConfig::numRemaining() const {
    return numRows * numCols - numMines;
}

float GameConfig::windowHeight() const {
    return numRows * BLOCK_SIZE;
}

float GameConfig::windowWidth() const {
    return numCols * BLOCK_SIZE;
}

unsigned int GameConfig::xToCoord(int x) const {
    // Bounds check, includes 0 for efficiency
    if (x <= 0) {
        return 0;
    } else if (x >= this->windowWidth()) {
        return numCols - 1;
    } else {
        return x / BLOCK_SIZE;
    }
}

unsigned int GameConfig::yToCoord(int y) const {
    if (y <= 0) {
        return 0;
    } else if (y >= this->windowHeight()) {
        return numRows - 1;
    } else {
        return y / BLOCK_SIZE;
    }
}

bool GameConfig::exists(unsigned int x, unsigned int y) const {
    return x >= 0 && y >= 0 && x < numCols && y < numRows;
}

}  // namespace cfg
