#include <iostream>
#include <vector>

#include "Mario.h"
#include "gameConfig.h"
#include "Game.h"

/* move
 * Controls Mario's movement based on the player's input and the current game state.
 * Handles walking, climbing, staying, and falling logic. Ensures Mario's actions
 * respect the board's layout and updates his position accordingly.
 * Parameters:
 *   - pressedKey: The key currently pressed by the player.
 *   - prevKey: The key previously pressed by the player.
 *   - tileBelow: The character representing the tile directly below Mario.
 *   - board: The game board, used to validate Mario's movement.
 */
void Mario::move(char& pressedKey, char& prevKey, char& beforePrevKey, char tileBelow, Board& board) {

    pos.draw(charAtPos);  // Draw Mario's initial position.

    // Handle falling logic.
    if (tileBelow == static_cast<char>(GameConfig::GameSigns::EMPTY)) {
        fallDistance++; // Increment fall distance counter.
        fall(pressedKey, prevKey, board, tileBelow); // Trigger fall behavior.
        return;
    }

    // Handle climbing up logic.
    if (pressedKey == 'w') {
        if (isLadderAt(board, 0, 0)) {
            pos.move(GameConfig::Direction::UP);

        }
        else if (isLadderAt(board, 0, 1)) {
            pos.move(GameConfig::Direction::UP);
            pressedKey = 's';  // Automatically stay after climbing.
        }
        else {
            jump(pressedKey, prevKey, board);// If no ladder, initiate jump.
            return;
        }


    }

    // Handle other directions
    switch (pressedKey) {
    case 'a': // Move left
        if (isWalkable(board, -1, 0)) {
            pos.move(GameConfig::Direction::LEFT);
            prevKey = 'a';
            beforePrevKey = 'a';

        }
        break;
    case 'd': // Move right
        if (isWalkable(board, 1, 0)) {
            pos.move(GameConfig::Direction::RIGHT);
            prevKey = 'd';
            beforePrevKey = 'd';
        }
        break;
    case 'x': // Move down a ladder
        if ((pos.getY() != BOTTOM_FLOOR) && (pos.getY() != BOTTOM_FLOOR_NO_FLOOR)) {
            if ((isLadderAt(board, 0, 0) && (tileBelow != static_cast<char>(GameConfig::GameSigns::FLOOR) && tileBelow != static_cast<char>(GameConfig::GameSigns::FLOOR_L) && tileBelow != static_cast<char>(GameConfig::GameSigns::FLOOR_R))
                || isLadderAt(board, 0, 1) || isLadderAt(board, 0, 2))) { // Check if Mario is on or above a ladder

                pos.move(GameConfig::Direction::DOWN);
                prevKey = 'x';
            }

        }
        break;
    case 's': //stay
        pos.move(GameConfig::Direction::STAY);
        prevKey = 's';
        break;
    default:
        break;

    }

    charAtPos = board.getChar(pos.getX(), pos.getY()); // Update the character at Mario's position.

}

/* jump
 * Handles Mario's jumping behavior based on the player's input and Mario's state.
 * Supports both stationary and directional jumps while ensuring boundaries.
 * Parameters:
 *   - pressedKey: The key currently pressed by the player.
 *   - prevKey: The key previously pressed by the player.
 *   - board: The game board, used to validate Mario's movement.
 */
void Mario::jump(char& pressedKey, char& prevKey, Board& board) {

    pos.draw(charAtPos);  // Draw Mario's initial position.

    if (prevKey == 's') {// Jump while stationary.
        if (isWithinBounds(board, 0, -2)) {
            pos.move(GameConfig::Direction::JUMP);
            pressedKey = 's';
        }
    }

    // Jump while moving.
    else {
        if (prevKey == 'a') {
            if (isWithinBounds(board, -2, -2) && !isWallAt(board, -2, -2)) {
                pos.move(GameConfig::Direction::LEFTUP);
                pressedKey = 'a';
            }

            else {
                pos.move(GameConfig::Direction::JUMP);
                pressedKey = 's';
                prevKey = 's';
            }
        }


        else if (prevKey == 'd') {
            if (isWithinBounds(board, 2, -2) && !isWallAt(board, 2, -2)) {
                pos.move(GameConfig::Direction::RIGHTUP);
                pressedKey = 'd';
            }
            else {
                pos.move(GameConfig::Direction::JUMP);
                pressedKey = 's';
                prevKey = 's';
            }


        }

        else {
            // Default to no movement.
            pos.move(GameConfig::Direction::STAY);
        }

    }
    charAtPos = board.getChar(pos.getX(), pos.getY());// Update the character at Mario's position.
    return;
}

/* fall
 * Handles Mario's falling behavior when he is above an empty tile or moving off a platform.
 * Adjusts Mario's position based on his previous movement direction.
 * Parameters:
 *   - pressedKey: The key currently pressed by the player.
 *   - prevKey: The key previously pressed by the player.
 *   - board: The game board, used to validate Mario's movement.
 *   - tileBelow: The character representing the tile directly below Mario.
 */
void Mario::fall(char& pressedKey, char& prevKey, Board& board, char tileBelow) {

    pos.draw(charAtPos);  // Draw Mario's initial position.

    switch (prevKey) {
    case 's':// Fall straight down.
        pos.move(GameConfig::Direction::DOWN);
        break;
    case 'a': // Fall diagonally left.
        if (isWalkable(board, -1, 1)) {
            pos.move(GameConfig::Direction::LEFTDOWN);
        }
        else {
            pos.move(GameConfig::Direction::DOWN);
        }
        break;
    case 'd':// Fall diagonally right.
        if (isWalkable(board, 1, 1)) {
            pos.move(GameConfig::Direction::RIGHTDOWN);
        }
        else {
            pos.move(GameConfig::Direction::DOWN);
        }
        break;
    }

    charAtPos = board.getChar(pos.getX(), pos.getY());// Update the character at Mario's position.
}

/*isWalkable
* Check if the cell (pos.x + offsetX, pos.y + offsetY) is walkable.*/
bool Mario::isWalkable(Board& board, int offsetX, int offsetY) const {
    // Check the cell (pos.x + offsetX, pos.y + offsetY) in the board
    int targetX = pos.getX() + offsetX;
    int targetY = pos.getY() + offsetY;
    return board.isWalkable(targetX, targetY);
}

/*isWithinBounds
* Check if the cell (pos.x + offsetX, pos.y + offsetY) is within the board bounds.*/
bool  Mario::isWithinBounds(Board& board, int offsetX, int offsetY) const {
    // Check if the cell (pos.x + offsetX, pos.y + offsetY) is within the board bounds
    int targetX = pos.getX() + offsetX;
    int targetY = pos.getY() + offsetY;
    return board.isWithinBounds(targetX, targetY);
}

/*isWallAt
* Check if the cell (pos.x + offsetX, pos.y + offsetY) contains a wall.*/
bool Mario::isWallAt(Board& board, int offsetX, int offsetY) const {
    // Check if the cell (pos.x + offsetX, pos.y + offsetY) contains a wall
    int targetX = pos.getX() + offsetX;
    int targetY = pos.getY() + offsetY;
    return board.isWallAt(targetX, targetY);
}