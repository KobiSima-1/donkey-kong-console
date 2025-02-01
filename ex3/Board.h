/* ------------------------- BOARD ---------------------------------------
 * Class: Board
 * Purpose: Represents a 2D game board and provides methods to interact with, modify, and query its state.
 */
#pragma once

#include "Point.h"

namespace fs = filesystem;

static constexpr int MAX_X = 80;   // Maximum width of the board
static constexpr int MAX_Y = 25;   // Maximum height of the board

class Board {
    // The board is represented as a 2D vector of characters
    vector<std::string> originalBoard;
    vector<std::string> currentBoard;

    Point marioStartPosition;          // Mario's initial starting position
    Point paulineStartPosition;          // pauline's initial starting position
    Point donkeyKongStartPosition;          // donkeykong's initial starting position
    Point legendStartPosition;          // legend's initial starting position
    Point HammerStartPosition;          // hammer's initial starting position
    vector<Point> ghostPositions;          //vector to store starting position of each ghost

    bool hammerExists = false;				              // Whether the hammer power-up exists
    bool illigalNumberOfEntities = false;		          // Whether the number of entities is correct
    bool getLevelFromText(const std::string& filename);   // Load the level from a text file

public:
    /*loadLevel
    * Loads the level from a text file.
    * Parameters:
    * filename:- The name of the file to load the level from.
    * Returns: - True if the level was loaded successfully, false otherwise.
    */
    bool loadLevel(const std::string& filename) {
        return getLevelFromText(filename);
    }
    void reset();
    void print() const;

    /*getChar
    * Retrieves the character at the given position on the board.*/
    char getChar(int x, int y) const { return currentBoard[y][x]; }

    /*getMarioInitPos
    * Retrieves the starting position of Mario on the board.*/
    Point getMarioInitPos() const { return marioStartPosition; }

    /*getDonkeyKongPos
    * Retrieves the starting position of Donkey Kong on the board.*/
    Point getDonkeyKongPos() const { return donkeyKongStartPosition; }

    /*getPaulinePos
    * Retrieves the starting position of Pauline on the board.*/
    Point getPaulinePos() const { return paulineStartPosition; }

    /*getLegendPos
    * Retrieves the starting position of Legend on the board.*/
    Point getLegendPos() const { return legendStartPosition; }

    /*getHammerPos
    * Retrieves the starting position of the hammer on the board.*/
    Point getHammerPos() const { return HammerStartPosition; }

    /*getHammerExists
    * Checks if the hammer power-up exists on the board.*/
    bool getHammerExists() const { return hammerExists; }

    /*getIlligalNumberOfEntities
    * Checks if the number of entities on the board is correct.*/
    bool getIlligalNumberOfEntities() const { return illigalNumberOfEntities; }

    /*getGhostPositions
    * Retrieves the starting positions of the ghosts on the board.
    * Returns: - A vector of Point objects representing the ghost positions.
    */
    std::vector<Point>& getGhostPositions() {
        return ghostPositions;
    }

    /*clearGhostPositions
    * Clears the vector of ghost positions.
    * This method is used to reset the game state when restarting the game.
    */
    void clearGhostPositions() {
        ghostPositions.clear();
    }

    /*isOnFloor
    * Checks if the given tile is a floor tile.
    * Parameters: - tile: The character representing the tile to check.
    * Returns: - True if the tile is a floor tile, false otherwise.
    */
    bool isOnFloor(char tile) const {
        return (tile == static_cast<char>(GameConfig::GameSigns::FLOOR)) || (tile == static_cast<char>(GameConfig::GameSigns::FLOOR_R)) || (tile == static_cast<char>(GameConfig::GameSigns::FLOOR_L));
    }

    bool checkCounters(int marioCount, int paulineCount, int donkeyKongCount, int legendCount);

    void fixObjectPos(Point& pos);

    Point getRandomePosition(Point pos);

    bool isWithinBounds(int x, int y) const;

    bool isLadderAt(int x, int y) const;

    bool isWalkable(int x, int y) const;

    bool isWallAt(int x, int y) const;
};
