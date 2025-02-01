/*  ------------------------- GAME ------------------------------------
 * Manages the game loop and interactions between entities and the board.
 * Handles Mario's movement, barrel logic, collision checks, and game state updates.
 * Prepares for additional entities, such as ghosts, to be added in future exercises.
 */
#pragma once

#include "Board.h"
#include "Point.h"
#include "Mario.h"
#include "Barrel.h"
#include "Ghost.h"
#include "SpecialGhost.h"
#include "Steps.h"
#include "Results.h"

static constexpr int ESC = 27;
static constexpr int ZERO = 0;
static constexpr int MAX_LIVE = 3;
static constexpr int MAX_WINS = 3;
static constexpr int MAX_BARRELS = 8;
static constexpr int MAX_GHOSTS = 6;
static constexpr int BARREL_SPAWN_INTERVAL = 3000; // Interval for spawning barrels (ms)
static constexpr int GHOST_SPAWN_INTERVAL = 6000; // Interval for spawning ghosts (ms)
static constexpr int GAME_UPDATE_INTERVAL = 150;   // Time increment for each game update (ms)
static constexpr int MARIO_MAX_FALL = 5;
static constexpr int BARREL_MAX_FALL = 8;
static constexpr int OFFSET_FOR_SCORE = 6;
static constexpr int OFFSET_FOR_LIVES = 19;
static constexpr int OFFSET_FOR_HAMMER = 28;

class Game {
    Board board;                       // The game board representation
    Mario mario;                       // Mario object to manage player movements
    Point pauline;                     // Pauline's position (victory target)
    Point donkeyKong;                  // Donkey Kong's position (static enemy)
    Point legend;					  // Legend's position (static enemy)
    Point hammer;                      // Hammer's position (power-up)

    std::vector<std::unique_ptr<Barrel>> barrels; // Dynamic allocation for barrels
    std::vector<std::unique_ptr<Ghost>> ghostVec; // Dynamic allocation for ghosts


    long randomSeed = 0;
    Steps steps;
    Results results;
	size_t iteration = 0;  // Current iteration of the game loop


    char pressedKey = static_cast<char>(GameConfig::GameSigns::EMPTY);  // Current key pressed by the player 
    char prevKey = static_cast<char>(GameConfig::GameSigns::EMPTY);     // Previous key pressed by the player
    char directionPrevKey = static_cast<char>(GameConfig::GameSigns::EMPTY); // Key pressed before the previous key
    int lives = MAX_LIVE;              // Number of lives Mario has
    bool won_level = false;            // Whether the player has won the level
    int numOfWins = 0;                 // Number of times the player has won
    bool running = false;              // Whether the game is currently running
    bool pause = false;                // Whether the game is paused

    int barrelSpawnCounter = 0;        // Counter to track when to spawn barrels
    char currentDirection = static_cast<char>(GameConfig::Direction::STAY);  // barrel's current direction

    int ghostSpawnCounter = 0;         // Counter to track when to spawn ghosts

    int fallCounter = 0;               // Tracks Mario's fall count

    int score = 0;                     // Player's score    

    bool hammerActive = false;         // Whether the hammer power-up is active

    bool hammerAnimationActive = false; // Add this variable to track animation state

public:
    std::string filename_prefix;
    std::string stepsFilename;
    std::string resultsFilename;
    std::string filename;

    Game();
	virtual ~Game() = default;

    // the function is virtual because it is used in the inherited classes
    virtual void setRandomSeed() = 0;
    virtual void run(const std::string& filename) = 0;
    virtual void updateGame() = 0;
    virtual void processInput() = 0;
    
    void pause_game();
	bool isValidInput();
    
    void printMessage(const std::string& message);
    void reset();
    void resetScore();
    void setEntityInitPos(Board& board);
    void setGhostsInitPos(Board& board);
    void printBoardErrorMessege();

    void marioDeath();
    void marioDeathAnimation();
    bool marioHitsBarrel();
    void marioFalling(char tileBelow);
    void marioGetHammer();
    void marioHammerUse();
    void marioHammerUseAnimation(int Xoffset);
    void barrelInHammerRange(int Xoffset);
    void ghostInHammerRange(int Xoffset);
    void killBarrel(size_t index);
    void killGhost(size_t index);
    void barrelDeathAnimation(Barrel* barrel);
    void ghostDeathAnimation(Ghost* ghost);



    bool barrel_hit_in_range_mario(Barrel* barrel, int expY, int expX);
    void updateBarrels();
    bool speedDrawBarrel();
    void addBarrel();
    void barrelLogic(size_t index, Barrel* barrel, Board& board);
    void removeBarrel(size_t index);
    bool barrelFalling(Barrel* barrel, char tileBelow);
    bool barrelHitsMario(Board& board, int marioX, int marioY);


    void drawInitialGhosts();

    void updateGhosts();
    void logicGhosts(size_t index, Ghost* ghost, Board& board);
    bool marioHitsGhost();
    bool ghostMeetGhost(size_t index, Ghost* ghost);
    bool ghostHitsMario(Board& board, Mario mario);
    void removeGhost(size_t index);

    void showLegend();

	//getters and setters in base class
	void initStepsAndResultsFiles(const std::string& filename); // Initialize the steps and results files
	long getRandomSeed() const { return randomSeed; } // Get the random seed
	void setRandomSeedInGame(long seed) { randomSeed = seed; } // Set the random seed
	Steps& getSteps() { return steps; } // Get the steps object
	Results& getResults() { return results; } // Get the results object
	Board& getBoard() { return board; } // Get the board object
	Point& getHammer() { return hammer; } // Get the hammer object
	Point& getPauline() { return pauline; } // Get the princess object
	Point& getDonkeyKong() { return donkeyKong; } // Get the Donkey Kong object
	Mario& getMario() { return mario; } // Get the Mario object
	void setResults(const Results& res) { results = res; } // Set the results object
	void setSteps(const Steps& st) { steps = st; } // Set the steps object
	void setFilename(const std::string& filename) { this->filename = filename; } // Set the filename
	bool getWonLevel() const { return won_level; } // Get the won_level boolean
	void setWonLevel(bool won) { won_level = won; } // Set the won_level boolean
	virtual bool getRunning() const { return running; } // Get the running boolean
	virtual void setRunning(bool run) { running = run; } // Set the running boolean
	bool getLives() const { return lives; } // Get the number of lives
	void setLives(int life) { lives = life; } // Set the number of lives
	void IncrementIteration() { ++iteration; } // Increment the iteration counter
	size_t getIteration() const { return iteration; } // Get the iteration counter
	void setIteration(size_t iter) { iteration = iter; } // Set the iteration counter
	char& getPressedKey() { return pressedKey; } // Get the pressed key
	char& getPrevKey() { return prevKey; } // Get the previous key
	char& getDirectionPrevKey() { return directionPrevKey; } // Get the direction previous key
	bool getHammerActive() const { return hammerActive; } // Get the hammerActive boolean
	bool getHammerAnimationActive() const { return hammerAnimationActive; } // Get the hammerAnimationActive boolean
	void setHammerAnimationActive(bool active) { hammerAnimationActive = active; } // Set the hammerAnimationActive boolean
	int getScore() const { return score; } // Get the score
	void setPressedKey(char key) { pressedKey = key; } // Set the pressed key

    /* victory
    *Ends the game with a victory message if Mario reaches Pauline's position.
    */
    void victory() {
        // Display the winning message and end the game
        printMessage("\n\
                     Congratulations!, You've saved Pauline!\n\
                              your Score: " + std::to_string(score));
        Sleep(150);
        numOfWins++;         // Increment the number of wins
        won_level = true;    // Set the level as won
        running = false;     // End the game
        reset();             // Reset the game
    }

    /* gameOver
     * Ends the game with a game over message if Mario has no lives left.
     */
    void gameOver() {
        std::cout << "Game Over! You have no lives left." << std::endl;
        running = false;
    }

 
    /* isAlive
     * Checks if Mario still has lives remaining.
     * Returns true if Mario is alive, otherwise false.
     */
    bool isAlive() const {
        return lives > 0;
    }

    /* checkVictory
     * Checks if Mario has reached Pauline's position.
     * If so, the game ends in victory.
     */
    void checkVictory() {
        // Check if Mario's position matches the princess's position
        if (pauline.getPosition() == mario.getPosition()) {
            score += 500;// Update score
            victory();
        }
    }

    /* checkIfHammerExists
    *gets getHammerExists boolean fron board.*/
    bool checkIfHammerExists() {
        return board.getHammerExists();
    }
};


