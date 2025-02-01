/*  ------------------------- HUMANE_INPUT_GAME ------------------------------------
 * The HumanInputGame class inherit frome class Game.
 * represents a game that allows the player to control Mario's movements using the keyboard.
 * The game can be saved at any point, and the player can resume the game from the last saved state.
 * The game loop is managed by the run() function, which processes player input, updates the game state, and displays the game board.
 */
#pragma once
#include "Game.h"

class HumanInputGame : public Game {
    bool isSaveMode = false;            // Whether the game is in save mode

public:
    HumanInputGame() : Game() {} // Explicitly calling the base constructor

	//function prototypes
    void run(const std::string& filename) override;
    void updateGame() override;
    void processInput() override;


	void setSaveMode(bool saveMode) {
		isSaveMode = saveMode;
	}


    /*setRandomSeed
	* before the game starts, set the random seed to the current time.
	* add the random seed to the steps file.
    */
    virtual void setRandomSeed() override {
        setRandomSeedInGame(static_cast<long>(std::chrono::system_clock::now().time_since_epoch().count()));
        getSteps().setRandomSeedToFile(getRandomSeed());
    }
};


