/*  ------------------------- FileInputGame ------------------------------------
 * This class is a subclass of Game. It represents a game that reads the player's input from a file, load mode.
 * The game loop is managed by the run() function, which processes player input, updates the game state, and displays the game board.
 * The game reads the player's input from a file and processes it as if it were entered by the player.
 *
 */
#pragma once
#include "Game.h"

class FileInputGame : public Game{
	size_t nextIteration = 0;
	bool isSilentMode = false; // Whether the game is in silent mode
	bool ifMatchingResultFound = true; // Whether the results file matches the steps file
public:
	using Game::Game;

	void run(const std::string& filename) override;
	void updateGame() override;
	void processInput() override;
	void setRandomSeed() override;


	void setSilentMode(bool _isSilentMode) {
		isSilentMode = _isSilentMode;
	}
};

