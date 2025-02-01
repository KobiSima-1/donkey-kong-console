#include <cctype>

#include "Point.h"
#include "Board.h"
#include "gameConfig.h"

/* move
 * Moves the Point in the specified direction based on the movement vector defined in GameConfig.
 * Parameters:
 *   - direction: The direction to move the Point (e.g., UP, DOWN, LEFT, RIGHT, etc.).
 * Updates the Point's X and Y coordinates accordingly.
 * return value: none.
 */
    void Point::move(GameConfig::Direction direction) {
    GameConfig::Movement movement = GameConfig::getMovementVector(direction);
    x += movement.dx;
    y += movement.dy;
}

/* drawStr
 * Draws a string to the current console position.
 * Parameters:
 *   - content: The string to render on the screen.
 * This function is useful for rendering longer messages or symbols at specific positions.
 * return value: none.
 */
void Point::drawStr(const std::string& content) {
    std::cout << content; // Print the content to the screen.
}

/* drawChar
 * Draws a single character to the current console position.
 * Parameters:
 *   - content: The character to render on the screen.
 * This function is optimized for rendering individual symbols or characters.
 * return value: none.
 */
void Point::drawChar(const char& content) {
    std::cout << content; // Print the content to the screen
}

/* drawAt (overloaded for a character)
 * Draws a single character at a specified position on the screen.
 * Parameters:
 *   - x: The X-coordinate where the character should be drawn.
 *   - y: The Y-coordinate where the character should be drawn.
 *   - content: The character to render at the specified position.
 * Moves the cursor to the target position, draws the character, and resets the cursor to the origin.
 * return value: none.
 */
void Point::drawAt(int x, int y, const char& content) {
    gotoxy(x, y);      // Move the cursor to the specified position.
    drawChar(content); // Draw the character at the position.
    gotoxy(0, 0);      // Reset the cursor position to the origin.

}

/* drawAt (overloaded for a string)
 * Draws a string at a specified position on the screen.
 * Parameters:
 *   - x: The X-coordinate where the string should start.
 *   - y: The Y-coordinate where the string should start.
 *   - content: The string to render at the specified position.
 * Moves the cursor to the target position, draws the string, and resets the cursor to the origin.
 * return value: none.
 */
void Point::drawAt(int x, int y, const std::string& content) {
    gotoxy(x, y);     // Move the cursor to the specified position.
    drawStr(content); // Draw the string at the position.
    gotoxy(0, 0);     // Reset the cursor position to the origin.
}