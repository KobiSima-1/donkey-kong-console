/*  ------------------------- POINT ------------------------------------
 * The Point class is responsible for managing and manipulating a single point's position
 * on the game board. It provides functionality for moving the point, drawing it on the screen,
 * erasing it, and interacting with other points or board elements. This class is a key utility
 * for controlling the movement and rendering of entities within the game environment.
 */
#pragma once

#include "Utils.h"
#include "gameConfig.h"

class Board; // Forward declaration
class Mario; // Forward declaration
class Barrel;// Forward declaration
class Ghost; // Forward declaration

class Point {
	char symbol; // Character representation on the board.
	int x, y;    // Coordinates of the point.


public:
	Point()
		: x(0),
		y(0),
		symbol(static_cast<char>(GameConfig::GameSigns::EMPTY)) {}

	/* Point
	 * Constructor to initialize the Point's coordinates and symbol.
	 * Parameters:
	 *   - _x: The X-coordinate of the point.
	 *   - _y: The Y-coordinate of the point.
	 *   - _symbol: The character symbol representing the point (default is ' ').
	 */
	Point(int _x, int _y, char _symbol = static_cast<char>(GameConfig::GameSigns::EMPTY)) : x(_x), y(_y), symbol(_symbol) {}


	/* draw (overloaded)
	 * Draws a specific character at the Point's current position.
	 * Parameters:
	 *   - c: The character to draw.
	 */
	void draw(char c) const {
		gotoxy(x, y);// Move cursor to the Point's position.
		std::cout << c;
	}

	/* draw
	 * Draws the Point's symbol at its current position.
	 */
	void draw() const {
		draw(symbol);
	}

	/* erase
	* Erases the Point by drawing a space character at its position.
	*/
	void erase() const {
		draw(static_cast<char>(GameConfig::GameSigns::EMPTY));
	}

	/* getPosition
	 * Returns the current position of the Point as a new Point object.
	 */
	Point getPosition() const {
		return *this;
	}


	/* operator==
	 * Compares two Point objects for equality based on their coordinates.
	 * Parameters:
	 *   - other: The Point to compare with.
	 * Returns:
	 *   - true if the two Points have the same coordinates; false otherwise.
	 */
	bool operator==(const Point& other) const {
		return x == other.x && y == other.y;
	}

	/* operator+
	 * Adds the coordinates of two Points and returns the result as a new Point.
	 * Parameters:
	 *   - other: The Point to add.
	 */
	Point operator+(const Point& other) const {
		return Point(x + other.x, y + other.y);
	}

	/* getX
	 * Returns the X-coordinate of the Point.*/
	int getX() const {
		return x;
	}

	/* getY
	 * Returns the Y-coordinate of the Point.*/
	int getY() const {
		return y;
	}

	/*getSymbol
	* Returns the symbol of the Point.*/
	char getSymbol() const {
		return symbol;
	}

	/*setX
	* Sets the X-coordinate of the Point.*/
	void setX(int _x) {
		x = _x;
	}

	/*setY
	* Sets the Y-coordinate of the Point.*/
	void setY(int _y) {
		y = _y;
	}

	/*setPosition
	* Sets the position of the Point.*/
	void setPosition(const Point& p) {
		x = p.getX();
		y = p.getY();
		symbol = p.getSymbol();
	}

	// Overloaded SetPosition that takes x,y,symbol
	void setPosition(int newX, int newY, char newSymbol) {
		x = newX;
		y = newY;
		symbol = newSymbol;
	}

	void drawChar(const char& content);

	void drawStr(const std::string& content);

	void drawAt(int x, int y, const std::string& content);

	void drawAt(int x, int y, const char& content);

	void move(GameConfig::Direction direction);

};