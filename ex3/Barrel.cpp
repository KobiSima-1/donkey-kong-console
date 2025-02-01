#include "Barrel.h"


/* move
 * Handles the barrel's movement on the board.
 * The barrel moves left or right depending on its direction and the tile below.
 * If the tile below is empty, the barrel falls. The direction may change if certain tiles are encountered.
 * Updates the barrel's position and redraws the board tile it moved from.
 */
void Barrel::moveBarrel(char tileBelow, Board& board) {

    drawPosition(); // Draw the barrel at its current position

    // Use Entity's move functionality
    move(tileBelow, board);

    // Handle falling logic
    if (tileBelow == static_cast<char>(GameConfig::GameSigns::EMPTY)) {
        fallDistance++;
        fall(board, tileBelow);
        return;
    }

    // stop movement if barrel reaches a wall.
    if (!isWithinBounds(board, 1, 0) || !isWithinBounds(board, -1, 0)) {
        movePosition(GameConfig::Direction::STAY);
        isStuck++;
    }

    setCharAtPos(board); // Redraw the character that was at the barrel's previous position
}


/* fall
 * Handles the barrel's falling behavior when the tile below is empty (' ').
 * Moves the barrel diagonally (left or right) as it falls and resets its height if it lands on a surface.
 * If the barrel's fall distance exceeds the explosion threshold, it triggers an explosion.
 * Updates the barrel's position and redraws the board tile it moved from.
 */
void Barrel::fall(Board& board, char tileBelow) {
    drawPosition(); // Draw the barrel at its current position


    // Move barrel while falling
    switch (getDirection()) {
    case false: // Falling left
        if (isWalkable(board, -1, 0)) {
            movePosition(GameConfig::Direction::LEFTDOWN);
        }
        else {
            movePosition(GameConfig::Direction::RIGHTDOWN);
        }
        break;
    case true: // Falling right
        if (isWalkable(board, 1, 0)) {
            movePosition(GameConfig::Direction::RIGHTDOWN);
        }
        else {
            movePosition(GameConfig::Direction::LEFTDOWN);
        }
        break;
    }

    // Update the character at the barrel's position
    setCharAtPos(board);
}

/* barrelExplosion
 * Handles the explosion logic for the barrel when it falls a sufficient distance.
 * Draws an explosion pattern on the board, temporarily replacing the surrounding area.
 * After a short delay, restores the original board state.
 * Returns true if the explosion occurred, false otherwise.
 */
void Barrel::barrelExplosion(char tileBelow, Board& board) {

    eraseEntity(); // Erase the barrel from its current position.

    Point currPos = getPositionEntity(); // Get the barrel's position

    // Explosion pattern
    std::vector<std::string> explosionPattern = {
        "*****",
        "** **",
        "*****"
    };

    // Store the original board state under the explosion
    int x = (int)explosionPattern[0].length(); // Width
    int y = (int)explosionPattern.size();  // Height
    std::vector<char> originalState;
    int count = 0;

    //case:barrel explodes on the bottom floor.
    if (!isWithinBounds(board, -1, 0)) {
        // Save the state of the board within the explosion area
        if (currPos.getY() == BOTTOM_FLOOR || currPos.getY() == BOTTOM_FLOOR_NO_FLOOR) {
            for (int i = -y / 2; i < y / 2 + 1; ++i) {
                for (int j = -x / 2 + 1; j <= (x / 2) + 1; j++) {
                    originalState.push_back(board.getChar(currPos.getX() + j, currPos.getY() + i));

                }
            }

        }

        else {
            // Save the state of the board within the explosion area
            for (int i = -y / 2; i <= y / 2 + 1; ++i) {
                for (int j = -x / 2 + 1; j <= (x / 2) + 1; j++) {
                    originalState.push_back(board.getChar(currPos.getX() + j, currPos.getY() + i));

                }
            }
        }
    }


    else if (!isWithinBounds(board, 1, 0)) {
        // Save the state of the board within the explosion area
        if (currPos.getY() == BOTTOM_FLOOR || currPos.getY() == BOTTOM_FLOOR_NO_FLOOR) {
            for (int i = -y / 2; i < y / 2 + 1; ++i) {
                for (int j = -x / 2 - 1; j < (x / 2); j++) {
                    originalState.push_back(board.getChar(currPos.getX() + j, currPos.getY() + i));

                }

            }
        }

        else {
            // Save the state of the board within the explosion area
            for (int i = -y / 2; i <= y / 2 + 1; ++i) {
                for (int j = -x / 2 - 1; j < (x / 2); j++) {
                    originalState.push_back(board.getChar(currPos.getX() + j, currPos.getY() + i));

                }

            }
        }
    }


    else if ((currPos.getY() == BOTTOM_FLOOR || currPos.getY() == BOTTOM_FLOOR_NO_FLOOR) && isWalkable(board, -1, 0) && isWalkable(board, 1, 0)) {
        // Save the state of the board within the explosion area
        for (int i = -y / 2; i < y / 2 + 1; ++i) {
            for (int j = -x / 2; j <= x / 2; j++) {
                originalState.push_back(board.getChar(currPos.getX() + j, currPos.getY() + i));

            }

        }
    }

    else {
        // Save the state of the board within the explosion area
        for (int i = -y / 2; i <= y / 2 + 1; ++i) {
            for (int j = -x / 2; j <= x / 2; j++) {
                originalState.push_back(board.getChar(currPos.getX() + j, currPos.getY() + i));

            }

        }
    }
    //case:barrel explodes on the bottom floor.
    if (!isWithinBounds(board, -1, 0)) {
        for (int i = 0; i < explosionPattern.size() - 1; ++i) {
            currPos.drawAt(currPos.getX() - 1, currPos.getY() + i, explosionPattern[i]);
        }
    }

    else if (!isWithinBounds(board, 1, 0)) {
        for (int i = 0; i < explosionPattern.size() - 1; ++i) {
            currPos.drawAt(currPos.getX() - 3, currPos.getY() + i, explosionPattern[i]);
        }
    }

    else {

        if (currPos.getY() == BOTTOM_FLOOR || currPos.getY() == BOTTOM_FLOOR_NO_FLOOR) {
            for (int i = 0; i < explosionPattern.size() - 1; ++i) {
                currPos.drawAt(currPos.getX() - 2, currPos.getY() + i, explosionPattern[i]);
            }
        }

        else {
            // Draw the explosion pattern on the board
            for (int i = 0; i < explosionPattern.size(); ++i) {
                currPos.drawAt(currPos.getX() - 2, currPos.getY() + i, explosionPattern[i]);
            }
        }
    }


    // Pause to display the explosion
    Sleep(40);

    //case:barrel explodes on the bottom floor.
    if (!isWithinBounds(board, -1, 0)) {
        // Save the state of the board within the explosion area
        if (currPos.getY() == BOTTOM_FLOOR || currPos.getY() == BOTTOM_FLOOR_NO_FLOOR) {
            for (int i = -y / 2; i < y / 2 + 1; ++i) {
                for (int j = -x / 2 + 1; j <= (x / 2) + 1; j++) {
                    currPos.drawAt(currPos.getX() + j, currPos.getY() + i, originalState.at(count));
                    count++;

                }

            }
        }

        else {
            for (int i = -y / 2; i <= y / 2 + 1; ++i) {
                for (int j = -x / 2 + 1; j <= (x / 2) + 1; j++) {
                    currPos.drawAt(currPos.getX() + j, currPos.getY() + i, originalState.at(count));
                    count++;

                }

            }
        }
    }

    else if (!isWithinBounds(board, 1, 0)) {
        // Save the state of the board within the explosion area
        if (currPos.getY() == BOTTOM_FLOOR || currPos.getY() == BOTTOM_FLOOR_NO_FLOOR) {
            for (int i = -y / 2; i < y / 2 + 1; ++i) {
                for (int j = -x / 2 - 1; j < (x / 2); j++) {
                    currPos.drawAt(currPos.getX() + j, currPos.getY() + i, originalState.at(count));
                    count++;

                }

            }
        }

        else {
            for (int i = -y / 2; i <= y / 2 + 1; ++i) {
                for (int j = -x / 2 - 1; j < (x / 2); j++) {
                    currPos.drawAt(currPos.getX() + j, currPos.getY() + i, originalState.at(count));
                    count++;

                }

            }
        }
    }

    else {


        if (currPos.getY() == BOTTOM_FLOOR || currPos.getY() == BOTTOM_FLOOR_NO_FLOOR) {
            // Save the state of the board within the explosion area
            for (int i = -y / 2; i < y / 2 + 1; ++i) {
                for (int j = -x / 2; j <= x / 2; j++) {
                    currPos.drawAt(currPos.getX() + j, currPos.getY() + i, originalState.at(count));
                    count++;

                }

            }
        }
        else {
            // Restore the original board state
            for (int i = -y / 2; i <= y / 2 + 1; ++i) {
                for (int j = -x / 2; j <= x / 2; j++) {
                    currPos.drawAt(currPos.getX() + j, currPos.getY() + i, originalState.at(count));
                    count++;
                }

            }
        }
    }


}


/* changeDirection
 * Changes the barrel's direction based on the tile it encounters.
 * If the barrel encounters '>' or '<', its direction changes accordingly.
 */
void Barrel::updateDirection(char tileBelow) {
    bool direction = getDirection();
    if (direction == false && tileBelow == static_cast<char>(GameConfig::GameSigns::FLOOR_R)) {
        changeDirection();
    }
    else if (direction == true && tileBelow == static_cast<char>(GameConfig::GameSigns::FLOOR_L)) {
        changeDirection();
    }
    return;

}