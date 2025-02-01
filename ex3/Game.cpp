#pragma once
#include <string>
#include <vector>
#include <cctype> 

#include "Game.h"


/* Game::Game
 * Description: Constructor for the Game class. Initializes the game board, characters (Mario, Pauline, Donkey Kong),
 * and other game state variables like the pressed key and starting positions.
 */
Game::Game()
    : board(),
    mario(),
    pauline(),
    donkeyKong(),
    legend(),
    hammer(),
    pressedKey(static_cast<char>(GameConfig::GameSigns::EMPTY)),
    prevKey(static_cast<char>(GameConfig::GameSigns::EMPTY)),
    directionPrevKey(static_cast<char>(GameConfig::GameSigns::EMPTY)),
    lives(MAX_LIVE),
    won_level(false),
    numOfWins(0),
    running(false),
    pause(false),
    score(0),
    barrelSpawnCounter(0),
    hammerActive(false),
    hammerAnimationActive(false),
    iteration(0),
    randomSeed(0) {}


bool Game::isValidInput() {
    for (auto c : GameConfig::keyPressed) {
        if (pressedKey == c) {
			return true;
		}
	}
	return false;
}

/*printBoardErrorMessege
* Description: Prints an error message if the board failed to load or was empty.
* The function checks if the board has an illegal number of entities or if the board was empty.
* If either condition is met, the function prints an error message to the console.
*/
void Game::printBoardErrorMessege() {
    if (board.getIlligalNumberOfEntities()) {
        printMessage("Error: Illegal number of entities in the level file.");
    }
    else {
        printMessage("Error: Level file was empty or failed to read.");
    }
}

/*Game::setEntityInitPos
Description: retrieves the starting positions of the entities from the provided
Board object and assigns those positions to the respective game entities.*/
void Game::setEntityInitPos(Board& board) {
    mario.setPosition(board.getMarioInitPos());  //  sets mario's position.
    pauline.setPosition(board.getPaulinePos());  //  sets pauline's position.
    donkeyKong.setPosition(board.getDonkeyKongPos());  //  sets donkey kong's position.
    legend.setPosition(board.getLegendPos());   //  sets legend position.
    if (checkIfHammerExists()) {
        hammer.setPosition(board.getHammerPos());  //  sets hammer position.
    }
    setGhostsInitPos(board);
    board.clearGhostPositions(); // clear initial positions for all ghost entities
}

/*setGhostsInitPos
* Description: Retrieves the starting positions of the ghosts from the provided
*   Board object and assigns those positions to the respective ghost entities.
* Parameters: - board
* Return Value: None.
*/
void Game::setGhostsInitPos(Board& board) {
    const auto& ghostPositions = board.getGhostPositions();

    ghostVec.clear();
    ghostVec.reserve(ghostPositions.size());

    for (const auto& gPos : ghostPositions) {
		char symbol = gPos.getSymbol();

		if (symbol == static_cast<char>(GameConfig::GameSigns::GHOST)) {
			// Create the Ghost on the heap , Set its position 
			auto ghostPtr = std::make_unique<Ghost>();
			ghostPtr->setPos(gPos);
			ghostVec.push_back(std::move(ghostPtr));
		}

		else if (symbol == static_cast<char>(GameConfig::GameSigns::SPECIAL_GHOST)) {
			// Create the Ghost on the heap , Set its position 
			auto ghostPtr = std::make_unique<SpecialGhost>();
			ghostPtr->setPos(gPos);
			ghostVec.push_back(std::move(ghostPtr));
		}
    }
	board.clearGhostPositions(); // clear initial positions for all ghost entities
}



/* Game::marioHitsBarrel
 * Description: Checks if Mario collides with any barrel in the game.
 * Return Value: True if Mario collides with a barrel, false otherwise.*/
bool Game::marioHitsBarrel() {
    for (const auto& barrel : barrels) { // Iterate through all barrels
        if (barrel->getPositionEntity() == mario.pos.getPosition()) { // Compare positions
            return true; // Mario hits a barrel
        }
    }
    return false; // No collision detected
}

/* Game::barrelHitsMario
 * Description: Checks if a specific barrel collides with Mario in an explosion radius.
 * Parameters:
 * - barrel: Pointer to the barrel being checked.
 * - expY, expX: Explosion radius in the Y and X directions.
 * Return Value: True if the barrel collides with Mario, false otherwise.
 */
bool Game::barrel_hit_in_range_mario(Barrel* barrel, int expY, int expX) {

    for (int i = -expY / 2; i <= expY / 2; i++) {
        for (int j = -expX / 2; j <= expX / 2; j++) {
            if ((barrel->getPositionEntity()).getX() + j == mario.pos.getX() && (barrel->getPositionEntity()).getY() + i == mario.pos.getY()) {
                return true;
            }
        }

    }
    return false;

}

/* Game::pause_game
 * Description: Pauses the game and waits for the ESC key to resume.
 */
void Game::pause_game() {
    GameConfig::ConsoleCenter::calculate();
    system("cls"); // Clear the screen
    gotoxy(GameConfig::ConsoleCenter::getX() - 8, GameConfig::ConsoleCenter::getY()); // 8 is an adjustment to x.
    std::cout << "Game Paused, press ESC again to continue.";

    while (true) {
        if (_kbhit()) {
            char nextKey = _getch();
            if (nextKey == pressedKey) {// Resume if ESC is pressed again
                system("cls"); // Clear the screen
                board.print(); // Reprint the current state of the board
                break;
            }
        }
    }
}


/* Game::speedDrawBarrel
 * Description: Determines whether it is time to spawn a new barrel based on a timer.
 * Return Value: True if a new barrel should be spawned, false otherwise.
 */
bool Game::speedDrawBarrel() {

    barrelSpawnCounter += GAME_UPDATE_INTERVAL;

    if (barrelSpawnCounter >= BARREL_SPAWN_INTERVAL) {
        barrelSpawnCounter = 0; // Reset the counter
        return true;            // It's time to spawn a barrel
    }
    return false; // Not yet time to spawn a barrel
}

/* Game::addBarrel
 * Description: Adds a new barrel to the game. Ensures the maximum number of barrels is not exceeded.
 */
void Game::addBarrel() {
    if (barrels.size() < MAX_BARRELS) {
        // Default starting position (to the left of Donkey Kong)
        Point startPos(donkeyKong.getX() - 1, donkeyKong.getY(), static_cast<char>(GameConfig::GameSigns::BARREL));

        // If running and we have existing barrels, we can use them to decide direction
        for (size_t i = 0; i < barrels.size() && running; ++i) {
            Barrel* barrel = barrels[i].get();

            // If we find a barrel whose direction initialization is true, we start to the right
            if (barrel->getDirection()) {
                startPos = Point(donkeyKong.getX() + 1, donkeyKong.getY(), static_cast<char>(GameConfig::GameSigns::BARREL));
                break;
            }
        }

        // Now that we have a valid startPos, create the new barrel
        barrels.push_back(std::make_unique<Barrel>(startPos));
    }
}

/* Game::marioDeath
 * Description: Handles Mario's death, decreases lives, and resets the game if lives remain.
 * Ends the game if no lives are left.
 */
void Game::marioDeath() {
    --lives; // Decrease lives
    if (score < 100) {
        score = 0;
    }
    else {
        score -= 100; // Decrease score 
    }

    if (isAlive()) {
        marioDeathAnimation();
        std::string death_message = "You died. Lives left: " + std::to_string(lives);
        printMessage(death_message);
        system("cls"); // Clear the screen
        board.print(); // Reprint the current state of the board
    }
    else {
        marioDeathAnimation();
        printMessage("Game Over. No lives left!\n\
                              Final Score: " + std::to_string(score));

        running = false;
    }
    reset();   // Reset the game state
}

/*marioDeathEnimation
* Description: Displays a death animation for Mario.
* The function erases and redraws Mario's position multiple times to simulate a blinking effect.
* The function also includes a delay between each erase-draw cycle to make the animation visible.
*/
void Game::marioDeathAnimation() {
    mario.pos.erase();
    Sleep(200);
    mario.pos.draw();
    Sleep(200);
    mario.pos.erase();
    Sleep(200);
    mario.pos.draw();
    Sleep(200);
    mario.pos.erase();
    Sleep(200);
    mario.pos.draw();
    Sleep(200);
}

/*barrelDeathAnimation
* Description: Displays a death animation for a barrel.
* The function erases and redraws the barrel's position multiple times to simulate a blinking effect.
* The function also includes a delay between each erase-draw cycle to make the animation visible.
*/

void Game::barrelDeathAnimation(Barrel* barrel) {
    barrel->eraseEntity();
    Sleep(200);
    barrel->drawEntity();
    Sleep(200);
    barrel->eraseEntity();
    Sleep(200);
}

/*ghostDeathAnimation
* Description: Displays a death animation for a ghost.
* The function erases and redraws the ghost's position multiple times to simulate a blinking effect.
* The function also includes a delay between each erase-draw cycle to make the animation visible.
*/

void Game::ghostDeathAnimation(Ghost* ghost) {
    ghost->eraseEntity();
    Sleep(200);
    ghost->drawEntity();
    Sleep(200);
    ghost->eraseEntity();
    Sleep(200);
}


/* Game::printMessage
 * Description: Clears the screen and displays a message centered on the console.
 * Parameters:
 * - message: The message to be displayed.
 */
void Game::printMessage(const std::string& message) {
    GameConfig::ConsoleCenter::calculate();
    system("cls"); // Clear the screen
    gotoxy(GameConfig::ConsoleCenter::getX(), GameConfig::ConsoleCenter::getY());
    Sleep(500); // Pause for the specified duration
    std::cout << message << std::endl;
    Sleep(500); // Pause for the specified duration
}

/* Game::resetAll
 * Description: Resets the game state.
 * initializes mario's position, clears all barrels, ghosts, and the hammer, and resets the score.
 */
void Game::reset() {
    mario.setPosition(board.getMarioInitPos());      // Reset Mario's position
    barrels.clear(); // Clear all barrels from the game
    hammerActive = false;
    pressedKey = prevKey = static_cast<char>(GameConfig::GameSigns::EMPTY); // Reset key presses
    resetScore(); // if no lives left, Resets the score
    
    if (!isAlive()) {
        lives = MAX_LIVE; // Reset lives
    }
}

/*resetScore
* Description: Resets the player's score to zero if no lives are left.
* return value: None.
*/
void Game::resetScore() {
    if (lives == ZERO) {   // If no lives are left, reset the score
        score = ZERO;
    }

    if (numOfWins == MAX_WINS) {  // If the player has won 3 times, reset the win count
        score = ZERO;
        numOfWins = ZERO;
    }

}




/* marioFalling
 * Description: Handles Mario's falling logic.
 * If Mario has fallen for more than 5 levels, it resets the fall and triggers his death.
 * Parameters: - tileBelow: The character representing the tile directly below Mario.
 * Return Value: None.
*/
void Game::marioFalling(char tileBelow) {

    if (mario.getFallDistance() >= MARIO_MAX_FALL && tileBelow != static_cast<char>(GameConfig::GameSigns::EMPTY)) {
        mario.setFallDisrance();
        marioDeath();
    }
    else if (mario.getFallDistance() < MARIO_MAX_FALL && tileBelow != static_cast<char>(GameConfig::GameSigns::EMPTY)) {
        mario.setFallDisrance();
    }
}

/*marioGetHammer
* Description: Checks if Mario picks up the hammer.
* If Mario's position is the same as the hammer's position, the function increments the score by 50 points.
* The function also sets the hammer as active and erases it from the board.
* Return Value: None.
*/
void Game::marioGetHammer() {
    if (hammer.getPosition() == mario.getPosition()) {
        if (!hammerActive) {
            score += 50;
        }
        hammerActive = true;
        hammer.drawChar(static_cast<char>(GameConfig::GameSigns::EMPTY));
    }
}


/* Game::marioHammerUse
 * Description: Handles Mario's hammer use logic.
 * If the hammer is active and the player presses the hammer key, the function triggers the hammer animation.
 * The function then checks the direction Mario is facing and calls the hammer use animation accordingly.
 * The function also checks if any barrels or ghosts are in the hammer's range and triggers their death animations.
 */
void Game::marioHammerUse() {
    if (hammerActive && pressedKey == static_cast<char>(GameConfig::GameSigns::HAMMER) && hammerAnimationActive == false) {
        pressedKey = static_cast<char>(GameConfig::Direction::STAY); // Reset the pressed key
        hammerAnimationActive = true; // Set the animation as active
        switch (prevKey) {
        case 'a':
            marioHammerUseAnimation(-2);
            barrelInHammerRange(-2);
            ghostInHammerRange(-2);
            break;
        case 'd':
            marioHammerUseAnimation(2);
            barrelInHammerRange(2);
            ghostInHammerRange(2);
            break;
        case 's':
            if (directionPrevKey == 'a') {
                marioHammerUseAnimation(-2);
                barrelInHammerRange(-2);
                ghostInHammerRange(-2);
            }
            else if (directionPrevKey == 'd') {
                marioHammerUseAnimation(2);
                barrelInHammerRange(2);
                ghostInHammerRange(2);
            }
            break;
        default:
            break;
        }
        hammerAnimationActive = false; // Reset the flag after the animation
    }
    return;
}

/*marioHammerUseAnimation
* Description: Displays a hammer use animation for Mario.
* The function erases and redraws the hammer's position multiple times to simulate a blinking effect.
*/
void Game::marioHammerUseAnimation(int Xoffset) {
    //save the char befor the animation
    char charAtPos1 = board.getChar(mario.pos.getX(), mario.pos.getY() - 1);
    char charAtPos2 = board.getChar(mario.pos.getX() + Xoffset, mario.pos.getY());


    mario.pos.drawAt(mario.pos.getX(), mario.pos.getY() - 1, static_cast<char>(GameConfig::GameSigns::EMPTY));
    mario.pos.drawAt(mario.pos.getX(), mario.pos.getY() - 1, static_cast<char>(GameConfig::GameSigns::HAMMER));
    Sleep(100);
    mario.pos.drawAt(mario.pos.getX(), mario.pos.getY() - 1, static_cast<char>(GameConfig::GameSigns::EMPTY));
    mario.pos.drawAt(mario.pos.getX() + Xoffset, mario.pos.getY(), static_cast<char>(GameConfig::GameSigns::HAMMER));
    Sleep(100);
    mario.pos.drawAt(mario.pos.getX() + Xoffset, mario.pos.getY(), static_cast<char>(GameConfig::GameSigns::EMPTY));

    mario.pos.drawAt(mario.pos.getX(), mario.pos.getY() - 1, charAtPos1);
    mario.pos.drawAt(mario.pos.getX() + Xoffset, mario.pos.getY(), charAtPos2);

    hammerAnimationActive = false; // Reset the flag after the animation
}

/*barrleInHammerRange
* Description: Checks if any barrels are in the hammer's range.
* If a barrel is in the hammer's range, the function triggers the barrel's death animation and removes the barrel.
* The function also increments the player's score by 100 points.
* parameters: - Xoffset: The X offset to check the hammer's range.
* return value: None.
*/
void Game::barrelInHammerRange(int Xoffset) {

    for (int i = static_cast<int>(barrels.size()) - 1; i >= 0; --i) {
        Barrel* barrel = barrels[i].get();
        if (barrel->getPositionEntity().getX() == mario.pos.getX() + Xoffset &&
            barrel->getPositionEntity().getY() == mario.pos.getY()) {
            barrelDeathAnimation(barrel);
            killBarrel(static_cast<size_t>(i));
            score += 100;
        }
    }

}

/*ghostInHammerRange
* Description: Checks if any ghosts are in the hammer's range.
* If a ghost is in the hammer's range, the function triggers the ghost's death animation and removes the ghost.
* The function also increments the player's score by 100 points.
* parameters: - Xoffset: The X offset to check the hammer's range.
* return value: None.
*/
void Game::ghostInHammerRange(int Xoffset) {

    for (int i = static_cast<int>(ghostVec.size()) - 1; i >= 0; --i) {
        Ghost* ghost = ghostVec[i].get();
        if (ghost->getPositionEntity().getX() == mario.pos.getX() + Xoffset &&
            ghost->getPositionEntity().getY() == mario.pos.getY()) {
            ghostDeathAnimation(ghost);
            killGhost(static_cast<size_t>(i));
            score += 100;
        }
    }

}

/*killBarrel
* Description: Removes a barrel from the game by index.
* parameters: - index: The index of the barrel to remove.
* return value: None.
*/
void Game::killBarrel(size_t index) {
    removeBarrel(index);
}

/*killGhost
* Description: Removes a ghost from the game by index.
* parameters: - index: The index of the ghost to remove.
* return value: None.
*/
void Game::killGhost(size_t index) {
    removeGhost(index);

}

/* Game::updateBarrels
 * Description: Updates the state of all barrels, including movement and interactions.
 * The function iterates through all barrels in the game and calls the barrelLogic function to update their state.
 * The function also checks if a barrel has fallen more than 8 levels and triggers its explosion.
 * The function also checks if a barrel hits Mario and triggers his death.
 * Return Value: None.
 */
void Game::updateBarrels() {
    // Iterate through all barrels
    for (size_t i = 0; i < barrels.size() && running; ++i) {
        // Get a pointer to the current barrel
        Barrel* barrel = barrels[i].get();

        // Erase the barrel from its current position
        barrel->eraseEntity();

        // Call the barrel logic to update movement and state
        barrelLogic(i, barrel, board);

        // Draw the barrel at its updated position
        if (i < barrels.size()) {
            // Check if the barrel still exists
            barrel = barrels[i].get();
            if (barrel) {
                barrel->drawEntity();
            }
        }
    }
}

/* Game::barrelLogic
 * Description: Handles the movement and collision logic for a single barrel.
 * Parameters:
 * - index: The index of the barrel in the vector.
 * - barrel: A pointer to the barrel object.
 * - board: The game board, used to validate Mario's movement.
 * return value: None.
 */
void Game::barrelLogic(size_t index, Barrel* barrel, Board& board) {
    char tileBelow = board.getChar((barrel->getPositionEntity()).getX(), (barrel->getPositionEntity()).getY() + 1);

    barrel->moveBarrel(tileBelow, board);

    if (barrelFalling(barrel, tileBelow)) {

        if (barrel_hit_in_range_mario(barrel, 3, 5)) {
            removeBarrel(index); // Remove barrel after explosion
            marioDeath();
        }

        else if (barrelHitsMario(board, mario.pos.getX(), mario.pos.getY())) {
            removeBarrel(index); // Remove barrel after hitting Mario
            marioDeath();
        }
        removeBarrel(index);
    }

    // Check if the barrel is stuck
    if (barrel->getisStuck() > 2 && barrel) {
        barrel->setisStuck();
        barrel->eraseEntity();

        removeBarrel(index);
    }

}

/* Game::removeBarrel
 * Description: Removes a barrel from the vector by index. Memory is automatically freed due to unique_ptr.
 * Parameters:
 * - index: The index of the barrel to remove.
 */
void Game::removeBarrel(size_t index) {
    // Remove the barrel from the vector; the unique_ptr will automatically delete the object
    barrels.erase(barrels.begin() + index);
}

/* Game::removeGhost
 * Description: Removes a Ghost from the vector by index. Memory is automatically freed due to unique_ptr.
 * Parameters:
 * - index: The index of the Ghost to remove.
 */
void Game::removeGhost(size_t index) {
    // Remove the ghost from the vector; the unique_ptr will automatically delete the object
    ghostVec.erase(ghostVec.begin() + index);
}


/* Game::barrelFalling
 * Description: Checks if a barrel fell more then 8 stores. if it did the barrel explodes, else it
 * resets fallDistance to 0.
 * Parameters:
 * - barrel: A pointer to the barrel object.
 * - tileBelow: The character representing the tile directly below Mario.
 * return value: True if the barrel has fallen more than 8 levels, false otherwise.
 */
bool Game::barrelFalling(Barrel* barrel, char tileBelow) {

    if (barrel->getFallDistance() >= BARREL_MAX_FALL && tileBelow != static_cast<char>(GameConfig::GameSigns::EMPTY)) {
        barrel->barrelExplosion(tileBelow, board);
        barrel->setFallDisrance();
        return true;
    }

    else if (barrel->getFallDistance() < BARREL_MAX_FALL && tileBelow != static_cast<char>(GameConfig::GameSigns::EMPTY)) {
        barrel->setFallDisrance();
    }
    return false;
}


/* get_barrel_hit_mario
 * Checks if Mario is hit by the barrel's explosion.
 * Returns true if Mario's position coincides with an explosion character ('*'), false otherwise.
 */
bool Game::barrelHitsMario(Board& board, int marioX, int marioY) {

    // Check if Mario's current position corresponds to a '*' on the game board
    if (board.getChar(marioX, marioY) == static_cast<char>(GameConfig::GameSigns::BOOM)) {
        // Mario is hit by the explosion
        return true;
    }

    // Mario is safe
    return false;
}


/*drawInitialGhosts
* Description: Draws all ghosts in the ghost vector at their initial positions.
* Return Value: None.
*/
void Game::drawInitialGhosts() {
    // Loop through each ghost in ghostVec and draw it
    for (auto& ghost : ghostVec) {
        ghost->drawEntity();
    }
}

/*updateGhosts
* Description: Updates the state of all ghosts in the game, including movement and interactions.
* The function iterates through all ghosts in the ghost vector and calls the logicGhosts function to update their state.
*/
void Game::updateGhosts() {
    // Iterate through all ghosts
    for (size_t i = 0; i < ghostVec.size() && running; ++i) {
        // Get a pointer to the current ghost
        Ghost* ghost = ghostVec[i].get();

        // Erase the ghost from its current position
        ghost->eraseEntity();

        // Call the ghost logic to update movement and state
        logicGhosts(i, ghost, board);

        if (i < ghostVec.size()) {
            ghost = ghostVec[i].get();
            if (ghost) {
                ghost->drawEntity();
            }
        }

        // Check if the ghost hits aother ghost after updating their positions
        for (size_t i = 0; i < ghostVec.size() && running; ++i) {
            // Get a pointer to the current ghost
            Ghost* ghost = ghostVec[i].get();
            // Check if the ghost meets another ghost
            ghostMeetGhost(i, ghost);
        }
    }
}

/*logicGhosts
* Description: Handles the movement and collision logic for a single ghost.
* Parameters:
* - index: The index of the ghost in the vector.
* - ghost: A pointer to the ghost object.
* - board: The game board, used to validate Mario's movement.
* Return Value: None.
*/
void Game::logicGhosts(size_t index, Ghost* ghost, Board& board) {
    // dummy tile below , ghost dont need it
	char tileBelow = board.getChar((ghost->getPositionEntity()).getX(), (ghost->getPositionEntity()).getY() + 1);

    ghost->moveGhost(tileBelow, board);

    // Check if the ghost hits Mario
    if (ghostHitsMario(board, mario)) {
        // If the ghost hits Mario, Mario dies and the game ends
        printMessage("Game Over , The wind won this time ");
        running = false;
        reset();
    }
}

/*marioHitsGhost
* Description: Checks if Mario collides with any ghost in the game.
* Return Value: True if Mario collides with a ghost, false otherwise.
*/
bool Game::marioHitsGhost() {
    for (const auto& ghost : ghostVec) { // Iterate through all barrels
        if (ghost->getPositionEntity() == mario.getPosition()) { // Compare positions
            return true; // Mario hits a barrel
        }
    }
    return false; // No collision detected
}

/*ghostMeetGhost
* Description: Checks if a ghost collides with another ghost in the game.
* Parameters:
* - index: The index of the ghost in the vector.
* - ghost: A pointer to the ghost object.
* Return Value: True if a ghost collides with another ghost, false otherwise.
*/
bool Game::ghostMeetGhost(size_t index, Ghost* ghost) {
    // Iterate over all ghosts in the vector
    for (size_t i = 0; i < ghostVec.size(); ++i) {
        // Skip the ghost at the given index to avoid self-comparison
        if (i == index) {
            continue;
        }

        // Get a pointer to the current ghost
        Ghost* currGhost = ghostVec[i].get();

        // Check if the positions match
        if (currGhost->getPositionEntity().getX() + 1 == ghost->getPositionEntity().getX() &&
            currGhost->getPositionEntity().getY() == ghost->getPositionEntity().getY()) {
            Sleep(10); // changed frpm 40. i did 40 : 5 to match the 150:5 = 30 of the save game.
            currGhost->changeDirection();
            ghost->changeDirection();

            return true;
        }
    }
    return false; // No collision detected
}

/* get_barrel_hit_mario
 * Checks if Mario is hit by the barrel's explosion.
 * parametrs:
 - board: The game board.
 * Returns true if Mario's position coincides with an explosion character ('*'), false otherwise.
 */
bool Game::ghostHitsMario(Board& board, Mario mario) {

    // Check if Mario's current position corresponds to the ghost
    if (board.getChar(mario.pos.getX(), mario.pos.getY()) == static_cast<char>(GameConfig::GameSigns::GHOST)) {
        // Mario is hit by the ghostY
        return true;
    }

    // Mario is safe
    return false;
}


/* Game::showLegend
 * Description: Displays the game legend, including score and lives.
 * return value: None.
 */
void Game::showLegend() {
    legend.erase(); // Erase the previous legend
    std::string line = "Score: " + std::to_string(score) + "  Lives: " + std::to_string(lives);
    std::string line2;

    // If hammer is active, append text
    if (hammerActive) {
        line2 = "Hammer: Active";
        legend.drawAt(legend.getX(), legend.getY(), line2);
    }

    legend.drawAt(legend.getX(), legend.getY() - 1, line);
}

void Game::initStepsAndResultsFiles(const std::string& filename) {
   filename_prefix = filename.substr(0, filename.find_last_of('.'));
   stepsFilename = filename_prefix + ".steps";
   resultsFilename = filename_prefix + ".result";
}
