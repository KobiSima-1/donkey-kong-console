#include "SpecialGhost.h"

/*isWalkable
* check if the special ghost can walk on the board.
* parametrs:
* @param board - the game board.
* @param xOffset - the x offset.
* @param yOffset - the y offset.
* @return bool - true if the special ghost can walk on the board, false otherwise.
*/
bool SpecialGhost::isWalkable(const Board& board, int xOffset, int yOffset) const {

	// Get the position of the entity
	Point pos = getPositionEntity();

	// Get the character at the position of the entity with the offset
	char cell = board.getChar(pos.getX() + xOffset, pos.getY() + yOffset);

	if (cell == static_cast<char>(GameConfig::GameSigns::LADDER)) {
		return true;
	}

	return Ghost::isWalkable(board, xOffset, yOffset);
}

/*isLadderAt
* check if there is a ladder at the special ghost's current position (or an offset position) on the board.
* parametrs:
* @param board - the game board.
* @param xOffset - the x offset.
* @param yOffset - the y offset.
* @return bool - true if there is a ladder at the special ghost's current position, false otherwise.
*/
bool SpecialGhost::isLadderAt(const Board& board, int xOffset, int yOffset) const {
	// Get the position of the entity
	if (!isWithinBounds(board, xOffset, yOffset)) {
		return false;
	}

	Point pos = getPositionEntity();

	// Get the character at the position of the entity with the offset
	char cell = board.getChar(pos.getX() + xOffset, pos.getY() + yOffset);

	return (cell == static_cast<char>(GameConfig::GameSigns::LADDER));
}



void SpecialGhost::moveGhost(char tileBelow, Board& board) {
	// Draw the ghost at its current position
	drawPosition();

	// Move the ghost
	//move(tileBelow, board);

	int temp = std::rand() % 20;  // Randomize direction


	if (temp > 17 && isLadderAt(board) && !isClimbingDown) {
		isClimbingUp = true;
	}

	if (isClimbingUp) {
		if (isOnFloor(tileBelow) && !isLadderAt(board, 0, 0)) {
			isClimbingUp = false;
		}
		else if (isWithinBounds(board, 0, -1) && isClimbingUp) {
			movePosition(GameConfig::Direction::UP);
		}
		else {
			isClimbingUp = false;
		}
		
	}
	

	// If there is a ladder at the ghost's current position

	if (temp < 2 && isLadderAt(board, 0, 2) && isOnFloor(tileBelow) && !isClimbingUp)  {
		isClimbingDown = true;
	}

	if (isClimbingDown) {
		if ((isOnFloor(tileBelow) && isLadderAt(board)) ||
			getPositionEntity().getY() == 23) {
			isClimbingDown = false;
		}
		else if (isWithinBounds(board, 0, 1) && isClimbingDown) {
			movePosition(GameConfig::Direction::DOWN);
		}
			
		else {
			isClimbingDown = false;
		}
		
	}

	if (!isClimbingUp && !isClimbingDown) {
		// Handle other directions
		if (getDirection()) { // Move right
			if (isWalkable(board, 1, 1)) {
				movePosition(GameConfig::Direction::RIGHT);
			}
			else {
				changeDirection();
			}
		}
		else { // Move left
			if (isWalkable(board, -1, 1)) {
				movePosition(GameConfig::Direction::LEFT);
			}
			else {
				changeDirection();
			}
		}

		// change movement if ghost reaches a wall.
		if (!isWithinBounds(board, 1, 0) || !isWithinBounds(board, -1, 0)) {
			changeDirection(); // Change direction if ghost hits a wall
		}

		// change movement if ghost	reaches the end of the floor.
		if (!isWalkable(board, -1, -1) || !isWalkable(board, 1, -1)) {
			updateDirection(tileBelow);
		}
	}


	setCharAtPos(board); // Redraw the character that was at the ghost's previous position
}








void SpecialGhost::updateDirection(char tileBelow) {
    // If you want custom direction logic (e.g., a 'climbing' direction), add it here.
    // Otherwise, you can just call the base class version.
    Ghost::updateDirection(tileBelow);
}