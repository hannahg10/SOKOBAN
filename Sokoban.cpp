/*
 * Author: Hannah Guillen
 * Course: COMP 2040 011
 * Last updated: June 26, 2023
 * 
 * Copyright 2023 Hannah Guillen
 */
#include <iostream>
#include <string>
#include <algorithm>
#include "Sokoban.hpp"
#include <SFML/Graphics.hpp>

Sokoban::Sokoban() : m_width(0), m_height(0),
 m_playerImage("sokoban/player_05.png") {}

void Sokoban::movePlayer(Direction direction) {
    std::pair<int, int> newPos = m_playerPos;
    std::pair<int, int> boxPos;

    // Calculate new position based on direction
    switch (direction) {
        case Direction::Up:
            newPos.first -= 1;
            boxPos = std::make_pair(newPos.first - 1, newPos.second);
            m_playerImage = "sokoban/player_08.png";  // Changed image
            break;
        case Direction::Down:
            newPos.first += 1;
            boxPos = std::make_pair(newPos.first + 1, newPos.second);
            m_playerImage = "sokoban/player_05.png";  // Changed image
            break;
        case Direction::Left:
            newPos.second -= 1;
            boxPos = std::make_pair(newPos.first, newPos.second - 1);
            m_playerImage = "sokoban/player_20.png";  // Changed image
            break;
        case Direction::Right:
            newPos.second += 1;
            boxPos = std::make_pair(newPos.first, newPos.second + 1);
            m_playerImage = "sokoban/player_17.png";  // Changed image
            break;
    }

    // Check new position
    switch (m_board[newPos.first][newPos.second]) {
        case CellType::WALL:
            // Blocked by wall, do nothing
            break;
        case CellType::BOX:
        case CellType::BOX_ON_STORAGE:
            // If box is in front, check next cell
            if (m_board[boxPos.first][boxPos.second] == CellType::EMPTY ||
                m_board[boxPos.first][boxPos.second] == CellType::STORAGE) {
                // Move box
                m_board[boxPos.first][boxPos.second] = (m_board[boxPos.first]
                 [boxPos.second] == CellType::STORAGE) ?
                  CellType::BOX_ON_STORAGE : CellType::BOX;
                // Move player
                m_board[newPos.first][newPos.second] = (m_board[newPos.first]
                 [newPos.second] == CellType::BOX_ON_STORAGE) ?
                  CellType::PLAYER_ON_STORAGE : CellType::PLAYER;
                m_board[m_playerPos.first][m_playerPos.second] = (m_board
                 [m_playerPos.first][m_playerPos.second] ==
                  CellType::BOX_ON_STORAGE || m_board[m_playerPos.first]
                  [m_playerPos.second] == CellType::PLAYER_ON_STORAGE)
                   ? CellType::STORAGE : CellType::EMPTY;
                m_playerPos = newPos;
            }
            break;
        case CellType::EMPTY:
        case CellType::STORAGE:
            // Move player
            m_board[newPos.first][newPos.second] = (m_board[newPos.first]
             [newPos.second] == CellType::STORAGE) ?
              CellType::PLAYER_ON_STORAGE : CellType::PLAYER;
            m_board[m_playerPos.first][m_playerPos.second] =
             (m_board[m_playerPos.first][m_playerPos.second] == CellType::
             PLAYER_ON_STORAGE) ? CellType::STORAGE : CellType::EMPTY;
            m_playerPos = newPos;
            break;
        default:
            break;
    }
}

bool Sokoban::isWon() {
    return std::all_of(m_board.begin(), m_board.end(),
     [](const std::vector<CellType>& row) {
        return std::all_of(row.begin(), row.end(), [](const CellType& cell) {
            return cell != CellType::STORAGE && cell !=
             CellType::PLAYER_ON_STORAGE;
        });
    });
}

int Sokoban::width() const { return m_width; }

int Sokoban::height() const { return m_height; }

void Sokoban::resetPlayerImage() {
    m_playerImage = "sokoban/player_05.png";
}

void Sokoban::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Texture playerTexture;
    sf::Sprite playerSprite;

    if (!playerTexture.loadFromFile(m_playerImage)) {
        std::cerr << "Error loading player texture from file! "
         << m_playerImage << '\n';
    }
    playerSprite.setTexture(playerTexture);

    for (int i = 0; i < m_height; ++i) {
        for (int j = 0; j < m_width; ++j) {
            std::string fileName;

            switch (m_board[i][j]) {
                case CellType::EMPTY:
                    fileName = "sokoban/ground_01.png";
                    break;
                case CellType::WALL:
                    fileName = "sokoban/block_06.png";
                    break;
                case CellType::BOX:
                    fileName = "sokoban/crate_03.png";
                    break;
                case CellType::STORAGE:
                    fileName = "sokoban/ground_04.png";
                    break;
                case CellType::PLAYER:
                    fileName = "sokoban/ground_01.png";
                    break;
                case CellType::BOX_ON_STORAGE:
                    fileName = "sokoban/crate_03.png";
                    break;
                case CellType::PLAYER_ON_STORAGE:
                    fileName = "sokoban/ground_04.png";
                    break;
            }

            if (!texture.loadFromFile(fileName)) {
                std::cerr << "Error loading texture from file! " <<
                 fileName << '\n';
                continue;
            }

            sprite.setTexture(texture);
            sprite.setPosition(j * 64.f, i * 64.f);  // tile size
            target.draw(sprite, states);

            // Draw player over the tile if player is on this tile
            if (m_board[i][j] == CellType::PLAYER || m_board[i][j] ==
             CellType::PLAYER_ON_STORAGE) {
                playerSprite.setPosition(j * 64.f, i * 64.f);  // tile size
                target.draw(playerSprite, states);
            }
        }
    }
}

std::istream& operator>>(std::istream& is, Sokoban& game) {
    is >> game.m_height >> game.m_width;
    game.m_board.resize(game.m_height, std::vector<CellType>(game.m_width));
    for (int i = 0; i < game.m_height; ++i) {
        for (int j = 0; j < game.m_width; ++j) {
            char cell;
            is >> cell;
            if (cell == '@') {
                game.m_playerPos = {i, j};
                game.m_board[i][j] = CellType::PLAYER;
            } else if (cell == '.') {
                game.m_board[i][j] = CellType::EMPTY;
            } else if (cell == '#') {
                game.m_board[i][j] = CellType::WALL;
            } else if (cell == 'A') {
                game.m_board[i][j] = CellType::BOX;
            } else if (cell == 'a') {
                game.m_board[i][j] = CellType::STORAGE;
            } else if (cell == '1') {
                game.m_board[i][j] = CellType::BOX_ON_STORAGE;
            } else if (cell == '2') {
                game.m_playerPos = {i, j};
                game.m_board[i][j] = CellType::PLAYER_ON_STORAGE;
            }
        }
    }
    return is;
}

