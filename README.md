# Sokoban
An implementation of Sokoban, a type of transport puzzle where the player pushes boxes or crates around in a warehouse to get them to storage locations.

## Description
This project is an implementation of Sokoban, a type of transport puzzle where the player pushes boxes or crates around in a warehouse to get them to storage locations. The game is command-line based where it is loaded from a text file passed as an argument during execution. The text file contains data for the game level layout, which includes the positions of the walls, boxes, and storage locations.

## Features
- It uses the SFML library for rendering the game state to screen. The game state includes the map and positions for the player, boxes, and storage locations. All game logic is handled within the game's main loop.
- The game also uses a two-dimensional grid to represent the game state, with different symbols representing different elements of the game. The grid is represented as a two-dimensional vector.

## How To Play
1. After compiling the program, an executable file called 'Sokoban' should be created.
2. All level files are inside the * sokoban * directory. To play a desired level, run the executable file followed by the level file in the command line, such as `./Sokoban level1.lvl`.
3. Use the arrows on your keyboard to move the player around the game board. Once all the boxes have been pushed to their desired areas by the player, the level will finish.
4. Each level is timed once any player movement is initiated.
5. You can press the `R` key at anytime to reset the board and time.
