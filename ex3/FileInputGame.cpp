#include "FileInputGame.h"

/* FileInputGame::run
 * Description: The main game loop. Handles initialization, input processing, game updates, and barrel spawning.
 * Parameters:
 * - filename: The name of the level file to load.
 * Return Value: None.
 */
void FileInputGame::run(const std::string& filename) {
    setRunning(true); // Ensure the game starts running
    ShowConsoleCursor(false); // Hide the blinking cursor
    setFilename(filename); // Set the filename for the current game



    initStepsAndResultsFiles(filename); // Get the steps and results filenames

    if (!getBoard().loadLevel(filename)) {  // Load the level from the file
        printBoardErrorMessege();     // Print an error message if the board failed to load
        return;
    }

    setEntityInitPos(getBoard());        // Set initial positions for all ghost entities 
    getBoard().reset();                  // Reset the board state
    getBoard().print();                    // Print the board
    

    if (getBoard().getHammerExists()) {
        getHammer().draw();
    }

    if (!isSilentMode) {
        showLegend();  // Draw the legend
    }

    setRandomSeed(); // Set the random seed
    srand(getRandomSeed());

    // Main game loop
    while (getRunning() && isAlive()) {
        if (isSilentMode && getResults().isFinishedBy(getIteration())) {
            getResults().reportResultError("Results file reached finish while game hadn't!", filename, getIteration());
            ifMatchingResultFound = false;
            break;
        }
        else {
			nextIteration = getResults().getNextIteration();
        }

        if(getSteps().isNextStepOnIteration(getIteration())) {
            setPressedKey(getSteps().popStep());
        }

        if (speedDrawBarrel()) {// Spawn barrels periodically
            addBarrel();
        }

        Sleep(isSilentMode ? 10 : 30);
        updateGame();           // Update the game state
        IncrementIteration();   // Increment the iteration counter
    }

    if (ifMatchingResultFound) {
        if (getResults().popResult() != std::pair{ getIteration(), Results::finished }) {
            getResults().reportResultError("Results file doesn't match finished event!", filename, getIteration());
            ifMatchingResultFound = false;
        }
		// Check if there are additional events after the finish event
        if (getResults().popResult().second != Results::noResult) {
            getResults().reportResultError("Results file has additional events after finish event!", filename, getIteration());
            ifMatchingResultFound = false;
        }
    }
}

/* FileInputGame::updateGame
 * Description: Updates the game state, including Mario's movement, collision detection, and victory checks.
 * The function also updates the barrel and ghost movements and interactions.
 * The function checks for collisions between Mario and barrels or ghosts and triggers the appropriate actions.
 * The function also checks if Mario has reached Pauline and ends the game if he has.
 * Return Value: None.
 */
void FileInputGame::updateGame() {
    
    getPauline().draw();
    getDonkeyKong().draw();
    

    int X = getMario().pos.getX();
    int Y = getMario().pos.getY();
    char tileBelow = getBoard().getChar(X, Y + 1);  // Tile directly below Mario


    // Update Mario's position
    getMario().pos.erase();  // Erase Mario from the current position
    getMario().move(getPressedKey(), getPrevKey(), getDirectionPrevKey(), tileBelow, getBoard());  // Move Mario based on input
    if (!isSilentMode) {
		getMario().pos.draw();  // Draw Mario at the new position
    }

    if (!isSilentMode) {
        showLegend();  // Draw the legend
    }

    if (!getHammerActive() && getBoard().getHammerExists()) {   // Draw the hammer if it exists
        getHammer().draw();
    }
    drawInitialGhosts();  // Draw the ghosts
    marioGetHammer();     // Check if Mario picks up the hammer

    marioHammerUse();     // Check if Mario uses the hammer

    // Check for collision with a barrel
    if (marioHitsBarrel()) {
        marioDeath();
    }


    if (marioHitsGhost()) {
        // If the Mario hits ghost, Mario dies and the game ends
        if (!isSilentMode) {
            printMessage("\n\
                        Game Over!,The wind won this time!\n\
                                Final Score: " + std::to_string(getScore()));
        }
        setLives(ZERO);
        setRunning(false);
        reset();

    }

    // Handle Mario's falling logic
    marioFalling(tileBelow);

    // Update barrel movements and interactions
    updateBarrels();

    // Update ghost movements and interactions
    updateGhosts();

    // Check for barrel collision with mario.
    if (marioHitsBarrel()) {
        marioDeath();
    }

    // Check for ghost collision with mario.    
    if (marioHitsGhost()) {
        // If the Mario hits ghost, Mario dies and the game ends
        if (!isSilentMode) {
            printMessage("\n\
                        Game Over!,The wind won this time!\n\
                            Final Score: " + std::to_string(getScore()));
        }

        setLives(ZERO);
        setRunning(false);
        reset();

    }

    // Check if Mario reaches Pauline
    if (getRunning()) {
        checkVictory();
    }

    // check if the result is correct
    if (isSilentMode && getResults().popResult() != std::pair{ getIteration(), Results::ResultValue::marioHitEnemy }) {
        getResults().reportResultError("Results file doesn't match hit bomb event!", filename, getIteration());
        ifMatchingResultFound = false;
        setRunning(false);
    }

    nextIteration = getResults().getNextIteration();

    if (isSilentMode && getIteration() == nextIteration) {
        getResults().reportResultError("Results file has a hit bomb event that didn't happen!", filename, getIteration());
        ifMatchingResultFound = false;
        setRunning(false);
    }
}

/* FileInputGame::processInput
 * Description: Checks for player keyboard input. If the ESC key is pressed, pauses the game.
 * If any other key is pressed, updates the pressedKey variable.
 * If the game is paused, waits for the ESC key to be pressed again to resume.
 * If the game is not paused, the pressed key is stored in the pressedKey variable.
 */
void FileInputGame::processInput() {
    //Check if any keys are pressed
    if (_kbhit()) {
        // Get the pressed key
        setPressedKey(_getch());
        // turn to lower letters.
        setPressedKey(std::tolower(static_cast<unsigned char>(getPressedKey())));

        if (getPressedKey() != 'p' && getHammerAnimationActive()) {
            setHammerAnimationActive(false); // Reset the flag if a different key is pressed
        }
    }
}

void FileInputGame::setRandomSeed() {
    setSteps(Steps::loadSteps(stepsFilename));
    setRandomSeedInGame(getSteps().getRandomSeed());
    setResults(Results::loadResults(resultsFilename));
}
