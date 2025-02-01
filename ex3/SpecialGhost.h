#pragma once
#include "Ghost.h"
#include "gameConfig.h"
#include "Board.h"
#include <random>


class SpecialGhost :public Ghost {
public:
	/* SpecialGhost (Constructor)
			innerit Ghost constructor.
				*/
	using Ghost::Ghost;

	bool isClimbingUp = false;
	bool isClimbingDown = false;

	/*isWalkable
		* check if the special ghost can walk on the board.*/
	bool isWalkable(const Board& board, int xOffset = 0, int yOffset = 0) const override;

	/*isOnFloor
		* check if the special ghost is on the floor.*/
	bool isOnFloor(char tile) const {
		return (tile == static_cast<char>(GameConfig::GameSigns::FLOOR)) ||
			(tile == static_cast<char>(GameConfig::GameSigns::FLOOR_R)) ||
			(tile == static_cast<char>(GameConfig::GameSigns::FLOOR_L));
	}

	/* isLadderAt
	 * Checks if there is a ladder at the ghost's current position (or an offset position) on the board.*/
	bool isLadderAt(const Board& board, int xOffset = 0, int yOffset = 0) const;

	//void updateLadderClimb(char tileBelow, Board& board);

	void moveGhost(char tileBelow, Board& board) override;
	void updateDirection(char tileBelow) override;

	friend class Game;
};


