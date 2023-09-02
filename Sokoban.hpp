/*
 * Author: Hannah Guillen
 * Course: COMP 2040 011
 * Last updated: June 14, 2023
 * 
 * Copyright 2023 Hannah Guillen
 */
#include <fstream>
#include <vector>
#include <utility>
#include <string>
#include <SFML/Graphics.hpp>

// Enumeration for direction
enum Direction { Up, Down, Left, Right };

// Enumeration for cell types
enum class CellType { EMPTY = '.', WALL = '#', BOX = 'A',
 STORAGE = 'a', PLAYER = '@', BOX_ON_STORAGE = '1', PLAYER_ON_STORAGE = '2' };

class Sokoban : public sf::Drawable {
 public:
    Sokoban();
    void movePlayer(Direction direction);
    void resetPlayerImage();
    bool isWon();
    int width() const;
    int height() const;
    friend std::istream& operator>>(std::istream& is, Sokoban& game);

 protected:
    void draw(sf::RenderTarget& target, sf::RenderStates
     states) const override;

 private:
    int m_width, m_height;
    std::pair<int, int> m_playerPos;
    std::vector<std::vector<CellType>> m_board;
    std::string m_playerImage;
};
