#pragma once

#include "gameConfig.h"
#include "Entity.h"

class Ghost :public Entity {
public:
	/* Ghost (Constructor)
		innerit Entity constructor.
	*/
	using Entity::Entity;

	/*isWalkable
	* check if the ghost can walk on the board.*/
	 virtual bool isWalkable(const Board& board, int xOffset = 0, int yOffset = 0) const override;

	/*isOnFloor
	* check if the ghost is on the floor.*/
	 virtual bool isOnFloor(char tile) const {
		return (tile == static_cast<char>(GameConfig::GameSigns::FLOOR)) || (tile == static_cast<char>(GameConfig::GameSigns::FLOOR_R)) || (tile == static_cast<char>(GameConfig::GameSigns::FLOOR_L));
	}

	virtual void moveGhost(char tileBelow, Board& board);
	virtual void updateDirection(char tileBelow) override;

	friend class Game;
};



