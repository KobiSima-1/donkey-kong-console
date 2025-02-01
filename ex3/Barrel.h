/*  ------------------------- BARREL ------------------------------------
 *The Barrel class represents the rolling barrels in the Donkey Kong game, simulating their movement, interactions with the game board
 *potential collisions with the player (Mario).
 */
#pragma once
#include "gameConfig.h"
#include "Entity.h"

static constexpr int EXPLOSION_DISTANCE = 8;
static constexpr int BOTTOM_FLOOR = 23;
static constexpr int BOTTOM_FLOOR_NO_FLOOR = 24;

class Barrel : public Entity {
	int height;  //starting height.
	int fallDistance;  //counting fall distance.
	int isStuck;   //counting how many frames is not moving. 


public:
	// Constructor
	Barrel(const Point& _startPos, char _charAtPos = static_cast<char>(GameConfig::GameSigns::EMPTY), bool _direction = true,
		int _height = 0, int _fallDistance = 0, int _isStuck = 0)
		: Entity(_startPos, _charAtPos, _direction), height(_height), fallDistance(_fallDistance), isStuck(_isStuck) {}


	/* getFallDistance
	 * Retrieves barrel's current fall distance.
	 * Returns:
	 *   - The fall distance as an integer.
	 */
	int getFallDistance() const {
		return fallDistance;
	}

	/* setFallDistance
	 * Resets barrel's fall distance counter to 0.
	 */
	void setFallDisrance() {
		fallDistance = 0;
	}

	/* getisStuck
 * Retrieves frames barrel's stuck near a wall.
 * Returns:
 *   - The number of frames as an integer.
 */
	int getisStuck() const {
		return isStuck;
	}

	/* setisStuck
	 * Resets barrel's isStuck counter to 0.
	 */
	void setisStuck() {
		isStuck = 0;
	}

	// control barrel's movement.
	void moveBarrel(char tileBelow, Board& board);

	void fall(Board& board, char tileBelow);

	//this function makes the explosion of a barrel.
	void barrelExplosion(char tileBelow, Board& board);

	// this function makes a barrel change its moving direction if it hits the wall.
	void updateDirection(char tileBelow) override;
};