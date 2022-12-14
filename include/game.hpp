#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "config.hpp"

namespace core {

struct NodeState {
    bool isMine;
    bool isRevealed;
    bool isFlagged;
    unsigned int neighborMines;
    NodeState();
};

class Node {
    NodeState state;
    sf::RectangleShape rect;
    sf::Text text;
    sf::Vector2u pos;

   public:
    Node(const unsigned int x, const unsigned int y, const cfg::GameConfig& config);
    void render(sf::RenderWindow& window);
    /* Getters */
    bool isMine() const;
    bool isRevealed() const;
    bool isFlagged() const;
    unsigned int numNeighborMines() const;
    bool isEmpty() const;
    /* Setters */
    void setMine();
    // Returns true if this operations reveals the cell, false otherwise
    bool reveal();
    void toggleFlag();
    void addNeighbor();
    // Gets a copy of the node's position
    sf::Vector2u getPos() const;
};

class Board {
    std::vector<Node> cells;
    const cfg::GameConfig config;
    unsigned int numRemaining;
    void generate(unsigned int resX, unsigned int resY);
    bool isStarted;
    bool gameover;
    // Converts an (x, y) point to a 1-D vector index
    unsigned int toIndex(unsigned int x, unsigned int y);

   public:
    Board(cfg::GameConfig config);
    void selectCell(unsigned int x, unsigned int y);
    void flagCell(unsigned int x, unsigned int y);
    bool isGameover() const;
    void render(sf::RenderWindow& window);
};

}  // namespace core

#endif
