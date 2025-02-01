/* ------------------------- ENTITY ---------------------------------------
 * The Entity class is an abstract base class that encapsulates the basic properties and behaviors of game entities like barrel and ghost,
 * such as position, direction, and interaction with the board. It provides a foundation for derived classes that
 * implement specific game entities.
 */
#pragma once
#include "Point.h"
#include "Board.h"

class Entity {
	Point pos;  //position on of a entity
	char charAtPos = static_cast<char>(GameConfig::GameSigns::EMPTY);  //
	bool direction = true; //true if right, false if left.

protected:
	// Randomly initialize direction
	void dirInit() {
		//std::srand(static_cast<unsigned int>(std::time(0))); // Seed random generator
		int temp = std::rand() % 2; // Generate a random number: 0 or 1
		direction = (temp == 0) ? false : true; // Randomize direction
	}

public:
	// Constructor
	Entity(Point startPos, char _charAtPos = static_cast<char>(GameConfig::GameSigns::EMPTY), bool _direction = true)
		: pos(startPos), charAtPos(_charAtPos), direction(_direction) {
		dirInit(); // Initialize the direction randomly
	}

	//empty constructor
	Entity() = default;

	// Destructor	
	virtual ~Entity() = default;

	// Change movement to opposite direction
	void changeDirection() {
		direction = !direction;
	}

	// Check if entity's current position is within bounds
	virtual bool isWithinBounds(const Board& board, int xOffset = 0, int yOffset = 0) const {
		return board.isWithinBounds(pos.getX() + xOffset, pos.getY() + yOffset);
	}

	// Check if entity's current position is walkable
	virtual bool isWalkable(const Board& board, int xOffset = 0, int yOffset = 0) const {
		return board.isWalkable(pos.getX() + xOffset, pos.getY() + yOffset);
	}
	// Draw entity on the board
	void drawEntity() const {
		pos.draw();
	}
	// Draw char on the board
	void drawPosition() const {
		pos.draw(charAtPos);
	}
	// Erase entity from the board
	void eraseEntity() {
		pos.erase();
	}
	// Erase char from the board
	void setCharAtPos(Board& board) {
		// Redraw the character that was at the barrel's previous position
		charAtPos = board.getChar(pos.getX(), pos.getY());
	}

	// control entity movement.
	virtual void move(char tileBelow, Board& board);

	// Getters

	/*getCharAtPos
	* Retrieves entity's current character.*/
	char getCharAtPos() const {
		return charAtPos;
	}

	/*getPositionEntity
	* Retrieves entity's current position.*/
	virtual Point getPositionEntity() const {
		return pos;
	}

	/* getDirection
	* Retrieves entity current direction.*/
	bool getDirection() const {
		return direction;
	}

	/*setPosition
	* Sets entity's current position.*/
	virtual void setPos(const Point& newPos) {
		pos = newPos;
	}

	/*updateDirection
	* Updates entity's direction based on the tile below.*/
	virtual void updateDirection(char tileBelow) = 0;

	/*movePosition
	* Moves entity's position based on the given direction.*/
	virtual void movePosition(GameConfig::Direction dir) {
		pos.move(dir);
	}
};

