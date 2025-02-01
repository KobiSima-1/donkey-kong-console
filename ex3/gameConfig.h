#pragma once
#include "Utils.h"

/*  ------------------------- GAME CONFIG ------------------------------------
 * Namespace: GameConfig
 * Purpose: Provides configuration constants and utility functions for the game,
 *          including movement directions, vectors, and console-related settings.
 */
namespace GameConfig {
    /*
     * Enum: Direction
     * Represents possible movement directions for game entities.
     */
    enum class Direction {
        UP,         // Move upward
        DOWN,       // Move downward
        LEFT,       // Move left
        RIGHT,      // Move right
        JUMP,       // Jump vertically
        LEFTUP,     // Move diagonally left and up
        RIGHTUP,    // Move diagonally right and up
        LEFTDOWN,   // Move diagonally left and down
        RIGHTDOWN,  // Move diagonally right and down
        STAY        // Stay in place
    };


    // Enum for entity symbols
  // Maps different game entities to their respective symbols on the board
    enum class GameSigns : char {
        MARIO = '@',        // Mario character
        PRINCESS = '$',     // Princess character
        BARREL = 'O',       // Barrel entity
        LADDER = 'H',       // Ladder
        FLOOR = '=',        // Flat floor
        FLOOR_L = '<',      // Floor slanting left
        FLOOR_R = '>',      // Floor slanting right
        EMPTY = ' ',        // Empty space
        DONKEY_KONG = '&',  // Donkey entity
        BOOM = '*',        // Explosion symbol
        LEGEND = 'L',       // Legend symbol
        GHOST = 'x',        // ghost symbol
		SPECIAL_GHOST = 'X', // special ghost symbol
        HAMMER = 'p',       // hammer symbol
        WALL = 'Q'          // wall symbol
    };
    /*
    * Struct: Movement
    * Represents a movement vector in the game.
    * Members:
    * - dx: Horizontal movement offset.
    * - dy: Vertical movement offset.
    */
    struct Movement {
        int dx; // Horizontal movement
        int dy; // Vertical movement
    };

    // Movement vectors for each direction
    static constexpr Movement movementVectors[] = {
        {0, -1},  // UP
        {0, 1},   // DOWN
        {-1, 0},  // LEFT
        {1, 0},   // RIGHT
        {0, -2},  // JUMP
        {-2, -2}, // LEFTUP
        {2, -2},  // RIGHTUP
        {-1, 1},  // LEFTDOWN
        {1, 1},   // RIGHTDOWN
        {0, 0}    // STAY
    };

    static constexpr char keyPressed[] = { 'w', 'a', 'x', 'd', 's' , 'p'};

    /*
    * Function: getMovementVector
    * Purpose: Retrieves the movement vector for a given direction.
    * Parameters:
    * - direction: The direction for which to get the movement vector.
    * Return Value: A `Movement` struct containing the dx and dy values for the specified direction.
    */
    inline constexpr Movement getMovementVector(Direction direction) {
        return movementVectors[static_cast<int>(direction)];
    }


    /*
     * Class: ConsoleCenter
     * Purpose: Provides utility functions for determining the center of the console window.
     * Members:
     * - x: Horizontal center of the console window.
     * - y: Vertical center of the console window.
     * Functions:
     * - calculate(): Calculates and sets the center coordinates of the console.
     * - getX(): Returns the horizontal center coordinate.
     * - getY(): Returns the vertical center coordinate.
     */
    class ConsoleCenter {
    private:
        static int x;
        static int y;

    public:
        /*
        * Function: calculate
        * Purpose: Calculates the center coordinates of the console window
        *          based on the current screen buffer size.
        */
        static void calculate() {
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
                x = csbi.srWindow.Right / 3;
                y = csbi.srWindow.Bottom / 2;
            }
        }

        static int getX() { return x; }
        static int getY() { return y; }
    };

}