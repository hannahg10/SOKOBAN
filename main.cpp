/*
 * Author: Hannah Guillen
 * Course: COMP 2040 011
 * Last updated: June 14, 2023
 * 
 * Copyright 2023 Hannah Guillen
 */

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include "Sokoban.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Sokoban Level 1:\n";
        return 1;
    }
    std::ifstream levelFile(argv[1]);
    if (!levelFile) {
        std::cout << "ERROR! Cannot open level file.\n";
        return 1;
    }

    Sokoban game;
    levelFile >> game;
    bool gameWon = false;

    sf::Time gameWinTime;

    // Create a SFML window
    sf::RenderWindow window(sf::VideoMode(game.width() * 64,
     game.height() * 64), "Sokoban");
    sf::Clock clock;
    sf::Font font;
    if (!font.loadFromFile("sokoban/SuperMario256.ttf")) {
        std::cerr << "Could not load font from file!" << std::endl;
        return 1;
    }

    sf::Text winText;
    winText.setCharacterSize(24);
    winText.setFont(font);
    winText.setFillColor(sf::Color::Green);

    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("sokoban/level_clear.wav")) {
        std::cout << "Error loading sound file!" << std::endl;
        return -1;
    }
    sf::Sound sound;
    sound.setBuffer(buffer);

    // Game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();

            // Handle other events
            } else if (event.type == sf::Event::KeyPressed) {
                if (!gameWon) {  // Check if game is not won yet
                    switch (event.key.code) {
                        case sf::Keyboard::W:
                        case sf::Keyboard::Up:
                            game.movePlayer(Up);
                           break;
                        case sf::Keyboard::A:
                        case sf::Keyboard::Left:
                            game.movePlayer(Left);
                            break;
                       case sf::Keyboard::S:
                       case sf::Keyboard::Down:
                            game.movePlayer(Down);
                            break;
                        case sf::Keyboard::D:
                        case sf::Keyboard::Right:
                            game.movePlayer(Right);
                            break;
                        default:
                            break;
                    }
                }

                if (event.key.code == sf::Keyboard::R) {
                    // Reset the level
                    levelFile.clear();  // Clear any flags
                    levelFile.seekg(0, std::ios::beg);
                     // Return to the start of the file
                    levelFile >> game;  // Reload the game
                    game.resetPlayerImage();  // Reset image
                    clock.restart();  // Restart the clock
                    gameWon = false;  // Reset the gameWon variable
                    gameWinTime = sf::Time();
                }
            }
        }

        // Calculate elapsed time
        sf::Time elapsed;
        if (gameWon) {
            elapsed = gameWinTime;
        } else {
            elapsed = clock.getElapsedTime();
        }
        int seconds = static_cast<int>(elapsed.asSeconds());
        int minutes = seconds / 60;
        seconds %= 60;

        // Format and display elapsed time
        std::ostringstream oss;
        oss << std::setw(2) << std::setfill('0') << minutes << ":"
         << std::setw(2) << std::setfill('0') << seconds;
        sf::Text text;
        text.setString(oss.str());
            text.setPosition(10, 10);
            text.setCharacterSize(24);
            text.setFont(font);
            text.setFillColor(sf::Color::Green);

            // Check if player has won
            if (game.isWon() && !gameWon) {
                gameWon = true;
                gameWinTime = clock.getElapsedTime();
                sound.play();
                winText.setString("You win!");
                winText.setPosition(game.width() * 32, game.height() * 32);

                // Draw the game
                window.clear();
                window.draw(game);
                window.display();
                continue;
            }

            // Draw the game
            window.clear();
            window.draw(game);
            window.draw(text);
            if (gameWon) {
                window.draw(winText);
            }
            window.display();
    }
    return 0;
}
