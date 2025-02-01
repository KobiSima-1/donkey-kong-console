#include "Ghost.h"

/*isWalkable
* check if the ghost can walk on the board.
* parametrs:
* @param board - the game board.
* @param xOffset - the x offset.
* @param yOffset - the y offset.
* @return bool - true if the ghost can walk on the board, false otherwise.
*/
bool Ghost::isWalkable(const Board& board, int xOffset, int yOffset) const {

	// Get the position of the entity
	Point pos = getPositionEntity();

	// Get the character at the position of the entity with the offset
	char cell = board.getChar(pos.getX() + xOffset, pos.getY() + yOffset);

	// Walkable cells be floor '=' or '<' or '>'	
	if (isOnFloor(cell)) {
		return true;
	}

	// If cell contains something else , it's not walkable
	return false;
}

/*moveGhost
* Move the ghost on the board.
* parametrs:
* @param tileBelow - the tile below the ghost.
* @param board - the game board.
* return value: none.
*/
void Ghost::moveGhost(char tileBelow, Board& board) {

	drawPosition(); // Draw the ghost at its current position

	move(tileBelow, board); // Move the ghost

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

	setCharAtPos(board); // Redraw the character that was at the ghost's previous position
}

/*updateDirection
* Randomly update ghost's direction.
* ghost wander on the floor with probability 0.95 of continuing in the same direction as their previous step, and 0.05 for changing direction
* parametrs:
* @param tileBelow - the tile below the ghost.
* return value: none.
*/
void Ghost::updateDirection(char tileBelow) {
	//std::srand(static_cast<unsigned int>(std::time(0))); // Seed random generator
	int temp = std::rand() % 20; // Generate a random number: 0 to 19
	if (temp == 0) {
		changeDirection(); // Change direction with 5% probability
	}
}
