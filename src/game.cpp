#include "game.hpp"

namespace core {

NodeState::NodeState()
    : isMine(false), isRevealed(false), isFlagged(false), neighborMines(0) {}

Node::Node(const unsigned int x, const unsigned int y, const cfg::GameConfig& config)
    : state(NodeState()), pos(sf::Vector2u(x, y)) {
    // Rectangle Shape
    const sf::Vector2f size(cfg::BLOCK_SIZE, cfg::BLOCK_SIZE);
    rect.setSize(size);
    rect.setOutlineThickness(4.0f);
    rect.setOutlineColor(sf::Color::Black);
    rect.setPosition(x * cfg::BLOCK_SIZE, y * cfg::BLOCK_SIZE);
    // Text Overlay
    text.setFont(*config.font);
    text.setString("");
    text.setPosition(x * cfg::BLOCK_SIZE + 10, y * cfg::BLOCK_SIZE);
    text.setColor(sf::Color::Black);
}

void Node::render(sf::RenderWindow& window) {
    if (this->isFlagged()) {
        this->rect.setFillColor(sf::Color::Yellow);
    } else if (this->isRevealed()) {
        if (this->isMine()) {
            this->rect.setFillColor(sf::Color::Red);
        } else {
            this->rect.setFillColor(sf::Color::White);
        }
    } else {
        this->rect.setFillColor(sf::Color::Blue);
    }
    window.draw(this->rect);
    window.draw(this->text);
}

bool Node::isMine() const {
    return this->state.isMine;
}

bool Node::isRevealed() const {
    return this->state.isRevealed;
}

bool Node::isFlagged() const {
    return this->state.isFlagged;
}

unsigned int Node::numNeighborMines() const {
    return this->state.neighborMines;
}

bool Node::isEmpty() const {
    return !this->isMine() && this->numNeighborMines() == 0;
}

void Node::setMine() {
    this->state.isMine = true;
}

bool Node::reveal() {
    if (!this->isRevealed() && !this->isFlagged()) {
        this->state.isRevealed = true;
        if (this->isMine()) {
            this->text.setString("X");
        } else if (!this->isEmpty()) {
            this->text.setString(std::to_string(this->numNeighborMines()));
        }
        return true;
    } else {
        return false;
    }
}

void Node::toggleFlag() {
    if (!this->isRevealed()) {
        this->state.isFlagged = !this->isFlagged();
        if (this->isFlagged()) {
            this->text.setString("!");
        } else {
            // Un-set flag string
            this->text.setString("");
        }
    }
}

void Node::addNeighbor() {
    this->state.neighborMines += 1;
}

sf::Vector2u Node::getPos() const {
    return this->pos;
}

Board::Board(cfg::GameConfig config)
    : config(config), isStarted(false), gameover(false), numRemaining(config.numRemaining()) {
    this->cells = std::vector<Node>();
    for (unsigned int col = 0; col < config.numCols; col++) {
        for (unsigned int row = 0; row < config.numRows; row++) {
            this->cells.push_back(Node(col, row, config));
        }
    }
}

void Board::render(sf::RenderWindow& window) {
    for (Node& n : this->cells) {
        n.render(window);
    }
}

void Board::generate(unsigned int resX, unsigned int resY) {
    // Calculate reserved index
    unsigned int res = toIndex(resX, resY);
    unsigned int maxVal = config.numCols * config.numRows;
    unsigned int numPlaced = 0;
    while (numPlaced < config.numMines) {
        unsigned int random = std::rand() % maxVal;
        if (random != res) {
            Node& mine = cells.at(random);
            // Don't double-count mines
            if (mine.isMine()) {
                continue;
            }
            mine.setMine();
            sf::Vector2u minePos = mine.getPos();
            // top-left
            if (config.exists(minePos.x - 1, minePos.y - 1)) {
                cells.at(this->toIndex(minePos.x - 1, minePos.y - 1)).addNeighbor();
            }
            // top-middle
            if (config.exists(minePos.x, minePos.y - 1)) {
                cells.at(this->toIndex(minePos.x, minePos.y - 1)).addNeighbor();
            }
            // top-right
            if (config.exists(minePos.x + 1, minePos.y - 1)) {
                cells.at(this->toIndex(minePos.x + 1, minePos.y - 1)).addNeighbor();
            }
            // middle-left
            if (config.exists(minePos.x - 1, minePos.y)) {
                cells.at(this->toIndex(minePos.x - 1, minePos.y)).addNeighbor();
            }
            // middle-right
            if (config.exists(minePos.x + 1, minePos.y)) {
                cells.at(this->toIndex(minePos.x + 1, minePos.y)).addNeighbor();
            }
            // bottom-left
            if (config.exists(minePos.x - 1, minePos.y + 1)) {
                cells.at(this->toIndex(minePos.x - 1, minePos.y + 1)).addNeighbor();
            }
            // bottom-middle
            if (config.exists(minePos.x, minePos.y + 1)) {
                cells.at(this->toIndex(minePos.x, minePos.y + 1)).addNeighbor();
            }
            // bottom-right
            if (config.exists(minePos.x + 1, minePos.y + 1)) {
                cells.at(this->toIndex(minePos.x + 1, minePos.y + 1)).addNeighbor();
            }
            numPlaced += 1;
        }
    }
    // Select random spots for mines
}

unsigned int Board::toIndex(unsigned int x, unsigned int y) {
    return x * config.numRows + y;
}

void Board::selectCell(unsigned int x, unsigned int y) {
    if (!this->isStarted) {
        this->generate(x, y);
        this->isStarted = true;
    }
    Node& n = cells.at(this->toIndex(x, y));
    // Ensure we actually reveal it (isn't flagged or anything...)
    if (n.reveal()) {
        // Update game state
        if (n.isMine()) {
            std::cout << "You selected the mine at: (" << std::to_string(n.getPos().x) << ", " << std::to_string(n.getPos().y) << ")... You Lose!" << std::endl;
            this->gameover = true;
        } else {
            this->numRemaining -= 1;
            if (this->numRemaining == 0) {
                std::cout << "You win!" << std::endl;
            }
        }
        // Reveal neighbors
        if (n.isEmpty()) {
            sf::Vector2u pos = n.getPos();
            // top-left
            if (config.exists(pos.x - 1, pos.y - 1)) {
                this->selectCell(pos.x - 1, pos.y - 1);
            }
            // top-middle
            if (config.exists(pos.x, pos.y - 1)) {
                this->selectCell(pos.x, pos.y - 1);
            }
            // top-right
            if (config.exists(pos.x + 1, pos.y - 1)) {
                this->selectCell(pos.x + 1, pos.y - 1);
            }
            // middle-left
            if (config.exists(pos.x - 1, pos.y)) {
                this->selectCell(pos.x - 1, pos.y);
            }
            // middle-right
            if (config.exists(pos.x + 1, pos.y)) {
                this->selectCell(pos.x + 1, pos.y);
            }
            // bottom-left
            if (config.exists(pos.x - 1, pos.y + 1)) {
                this->selectCell(pos.x - 1, pos.y + 1);
            }
            // bottom-middle
            if (config.exists(pos.x, pos.y + 1)) {
                this->selectCell(pos.x, pos.y + 1);
            }
            // bottom-right
            if (config.exists(pos.x + 1, pos.y + 1)) {
                this->selectCell(pos.x + 1, pos.y + 1);
            }
        }
    }
}

void Board::flagCell(unsigned int x, unsigned int y) {
    Node& n = cells.at(this->toIndex(x, y));
    n.toggleFlag();
}

bool Board::isGameover() const {
    return this->gameover && this->numRemaining > 0;
}

}  // namespace core
