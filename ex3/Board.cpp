#include "Board.h"
#include <cstring>
#include <iostream>

#include "Board.h"



/* getLevelFromText
 * Reads a level from a text file and stores it in the `originalBoard` vector.
 * Returns true if the level was successfully loaded, false otherwise.
 */
bool Board::getLevelFromText(const std::string& filename) {
	originalBoard.clear();
	hammerExists = false;				 // Whether the hammer power-up exists
	illigalNumberOfEntities = false;		 // Whether the number of entities is correct

	int marioCounter = 0;				 // Counter to keep track of number of marios
	int paulineCounter = 0;				 // Counter to keep track of number of paulines
	int donkeyKongCounter = 0;				 // Counter to keep track of number of donkeyKongs
	int legendCounter = 0;				 // Counter to keep track of number of legends

	// Open the file
	std::ifstream levelFile(filename);

	if (!levelFile) {
		std::cerr << "Failed to open file: " << filename << std::endl;
		return false;
	}

	int row = 0;
	std::string line;
	while (std::getline(levelFile, line)) {
		// Check if the line length is within the allowed range
		if (line.length() < MAX_X) {
			line.insert(line.size() - 1, MAX_X - line.size(), static_cast<char>(GameConfig::GameSigns::EMPTY));
		}

		else if (line.length() > MAX_X) {
			line.erase(MAX_X, line.length() - MAX_X);
		}

		// Scan the line for special characters
		 // Iterate over each character in the line
		for (int col = 0; col < static_cast<int>(line.size()); ++col) {
			char symbol = line[col];

			// Create a Point for this character
			Point p(col, row, symbol);
			// Found Mario
			if (symbol == static_cast<char>(GameConfig::GameSigns::MARIO)) {
				marioStartPosition.setPosition(p);
				marioCounter++;
				line[col] = static_cast<char>(GameConfig::GameSigns::EMPTY);
			}
			else if (symbol == static_cast<char>(GameConfig::GameSigns::GHOST) || 
				symbol == static_cast<char>(GameConfig::GameSigns::SPECIAL_GHOST)) {
				// Found a ghost, push it into our ghosts vector
				ghostPositions.push_back(p);
				line[col] = static_cast<char>(GameConfig::GameSigns::EMPTY);
			}
			else if (symbol == static_cast<char>(GameConfig::GameSigns::DONKEY_KONG)) {
				// Found Donkey Kong
				donkeyKongStartPosition.setPosition(p);
				donkeyKongCounter++;
			}
			else if (symbol == static_cast<char>(GameConfig::GameSigns::PRINCESS)) {
				// Found Pauline
				paulineStartPosition.setPosition(p);
				paulineCounter++;
			}
			else if (symbol == static_cast<char>(GameConfig::GameSigns::LEGEND)) {
				//	Found Legend
				legendStartPosition.setPosition(p);
				legendCounter++;
			}
			else if (symbol == static_cast<char>(GameConfig::GameSigns::HAMMER)) {
				// Found a hammer
				HammerStartPosition.setPosition(p);
				hammerExists = true;
				line[col] = static_cast<char>(GameConfig::GameSigns::EMPTY);
			}

		}

		originalBoard.push_back(line);
		++row;
	}

	size_t width = originalBoard[0].size(); // Get the width of the board
	originalBoard.push_back(std::string(width, static_cast<char>(GameConfig::GameSigns::FLOOR))); // Add a bottom floor

	if (!checkCounters(marioCounter, paulineCounter, donkeyKongCounter, legendCounter)) {
		return false;
	}

	// Fix the position of the objects , if needed
	fixObjectPos(paulineStartPosition);
	fixObjectPos(marioStartPosition);
	fixObjectPos(donkeyKongStartPosition);
	for (Point& ghostPos : ghostPositions) {
		fixObjectPos(ghostPos);
	}


	if (originalBoard.empty()) {
		std::cerr << "Level file was empty or failed to read: " << filename << std::endl;
		return false;
	}

	// If everything worked, return true
	return true;
}


/* reset
 * Resets the current board to its original state.
 * Copies the `originalBoard` to `currentBoard` row by row.
 */
void Board::reset() {
	currentBoard = originalBoard;
}

/* print
 * Prints the current state of the board to the console.
 * Outputs each row of `currentBoard` except the last one with a newline,
 * and the last row without a newline.
 */
void Board::print() const {
	for (size_t i = 0; i < currentBoard.size() - 1; ++i) {
		std::cout << currentBoard[i];
		if (i != currentBoard.size() - 1) {
			std::cout << std::endl;
		}
	}
}

/* isWithinBounds
 * Checks if a given (x, y) coordinate is within the bounds of the board.
 * Returns true if the coordinates are within the board limits, false otherwise.
 */
bool Board::isWithinBounds(int x, int y) const {
	return (y > 0 && y < static_cast<int>(currentBoard.size()) &&
		x > 0 && x < static_cast<int>(currentBoard[y].size()) && !isWallAt(x, y));
}

/* isLadderAt
 * Checks if a ladder ('H') is present at the given (x, y) position on the board.
 * Returns true if the position contains a ladder and is within bounds, false otherwise.
 */
bool Board::isLadderAt(int x, int y) const {
	// Check if within bounds
	if (!isWithinBounds(x, y)) {
		return false;
	}

	return currentBoard[y][x] == static_cast<char>(GameConfig::GameSigns::LADDER);
}


/* isWalkable
 * Checks if the cell at the given (x, y) position is walkable.
 * Walkable cells include empty spaces (' '), ladders ('H'), coins ('$'), or power-ups ('&').
 * Returns true if the cell is walkable and within bounds, false otherwise.
 */
bool Board::isWalkable(int x, int y) const {
	// Check if within bounds
	if (!isWithinBounds(x, y)) { return false; }

	char cell = currentBoard[y][x];

	// Walkable cells might be floor ' ' or ladder 'H'
	if (cell != static_cast<char>(GameConfig::GameSigns::WALL) &&
		cell != static_cast<char>(GameConfig::GameSigns::FLOOR) &&
		cell != static_cast<char>(GameConfig::GameSigns::FLOOR_L) &&
		cell != static_cast<char>(GameConfig::GameSigns::FLOOR_R)) {
		return true;
	}

	// If cell contains something else (e.g., a block 'x'), it's not walkable
	return false;
}

/*isWallAt
* Checks if the cell at the given (x, y) position is a wall.
* Returns true if the cell is a wall, false otherwise.
*/
bool Board::isWallAt(int x, int y) const {
	return currentBoard[y][x] == static_cast<char>(GameConfig::GameSigns::WALL);
}

/*checkCounters
* The function will check if the number of entities on the board is correct.
* Returns true if the number of entities is correct, false otherwise.
*/
bool Board::checkCounters(int marioCount, int paulineCount, int donkeyKongCount, int legendCount) {
	if (marioCount == 1 && paulineCount == 1 && donkeyKongCount == 1 && legendCount == 1) {
		return true;
	}
	else {
		illigalNumberOfEntities = true;
		return false;
	}

}


/*fixObject
* The function will fix the object on the board
* If the object is not on the floor, we will find a new position
*/
void Board::fixObjectPos(Point& pos) {
	char currX = pos.getX();
	char currY = pos.getY();
	char tileBelow = originalBoard[pos.getY() + 1][pos.getX()];
	// Check if the object is on the floor , if not we will find a new position
	if (!isOnFloor(tileBelow)) {
		pos = getRandomePosition(pos); // replace the object with a new position
		originalBoard[currY][currX] = static_cast<char>(GameConfig::GameSigns::EMPTY); // Clear the old position on the board
	}
}

/*getRandomePosition
* The function will plot an x ​​y point from the board.
*We will check if there is a floor below, if so we will return the point
*otherwise we will grill a point on the board again
*/
Point Board::getRandomePosition(Point pos) {
	// Randomly generate a position for the ghost
	while (true) {
		int x = rand() % (MAX_X - 1);// Random x position
		int y = rand() % (MAX_Y - 1);// Random y position

		char tileBelow = originalBoard[y + 1][x];
		// Check if the position is a floor tile
		if (isOnFloor(tileBelow)) {
			Point startPos = Point(x, y, pos.getSymbol());
			return startPos;
		}
	}
}



