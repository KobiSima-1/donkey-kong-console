/*  ------------------------- MARIO ------------------------------------
 *   The Mario class represents the main character in the game. It is responsible for managing Mario's
 *   position, movement, and interactions with the game board. The class handles Mario's drawing and
 *   erasing on the screen, movement logic (walking, jumping, falling), and checks for interactions with
 *   game elements such as ladders and walkable tiles.
 */
#pragma once
#include "Point.h"
#include "Board.h"

class Board;

class Mario {
	Point pos;              // Mario's current position, represented by the Point class.
	char charAtPos;   // The character present at Mario's position.
	int fallDistance;       // Counter for tracking Mario's fall distance.

public:

	/* Mario (Constructor)
	 * Initializes a Mario object with a starting position, the character at that position,
	 * and an initial fall distance (default is 0).
	 */
	Mario(char _charAtPos = static_cast<char>(GameConfig::GameSigns::EMPTY), int _fallDistance = 0)
		:pos(0, 0, static_cast<char>(GameConfig::GameSigns::MARIO)), charAtPos(_charAtPos), fallDistance(_fallDistance) {}


	/* getFallDistance
	 * Retrieves Mario's current fall distance.*/
	int getFallDistance() const {
		return fallDistance;
	}

	Point getPos() {
		return pos;
	}

	/* setFallDistance
	 * Resets Mario's fall distance counter to 0.*/
	void setFallDisrance() {
		fallDistance = 0;
	}

	/*setPosition
	* sets mario's position on the current board.*/
	void setPosition(const Point& p) {
		// Let the internal pos copy p
		pos.setPosition(p);
	}

	/*setPosition
	* sets mario's position on the current board.*/
	void setPosition(int x, int y, char symbol) {
		// Let the internal pos set (x,y,symbol)
		pos.setPosition(x, y, symbol);
	}

	/*getPosition
	* Retrieves Mario's current position.*/
	const Point& getPosition() const {
		return pos;
	}

	/* isLadderAt
	 * Checks if there is a ladder at Mario's current position (or an offset position) on the board.
	 * Parameters:
	 *   - board: The game board to check for a ladder.
	 *   - xOffset: Optional X offset to check a relative position (default is 0).
	 *   - yOffset: Optional Y offset to check a relative position (default is 0).
	 * Returns:
	 *   - true if there is a ladder at the position; false otherwise.
	 */
	bool isLadderAt(const Board& board, int xOffset = 0, int yOffset = 0) const {
		return board.isLadderAt(pos.getX() + xOffset, pos.getY() + yOffset);
	}

	/* isWalkable
	 * Checks whether Mario's current position (or an offset position) is walkable on the board.
	 * Parameters:
	 *   - board: The game board to check walkability against.
	 *   - xOffset: Optional X offset to check a relative position (default is 0).
	 *   - yOffset: Optional Y offset to check a relative position (default is 0).
	 * Returns:
	 *   - true if the position is walkable; false otherwise.
	 */
	bool isWalkable(Board& board, int offsetX, int offsetY) const;

	bool isWithinBounds(Board& board, int offsetX, int offsetY) const;

	bool isWallAt(Board& board, int offsetX, int offsetY) const;


	// control Mario's movement.
	void move(char& pressedKey, char& prevKey, char& beforePrevKey, char tileBelow, Board& board);

	void jump(char& pressedKey, char& prevKey, Board& board);

	void fall(char& pressedKey, char& prevKey, Board& board, char tileBelow);

	friend class Game;
	friend class HumanInputGame;
	friend class FileInputGame;
};