#include "Menu.h"
#include <iostream>
#include <conio.h>


/*get_all_screens
* Get all the screen files in the current directory
* Parameters:
*  - screens: A vector of strings to store the screen files
* Return value: none
*/
void Menu::get_all_screens(vector<string>& screens) {
    screens.clear();
    // Current directory
    fs::path currentDir = fs::current_path();

    try {
        for (const auto& entry : fs::directory_iterator(currentDir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".screen") {
                screens.push_back(entry.path().filename().string());
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << '\n';
    }

    std::sort(screens.begin(), screens.end());
}

/*playAllBoards
* Play all the boards in the current directory
* Parameters:
* - game: A reference to the Game object
* return value: none
*/
void Menu::playAllBoards() {
    if (screens.empty()) {
        std::cerr << "Not valid .screen files found in the directory.\n";
        return;
    }

    for (const auto& screen : screens) {
        std::cout << "Playing board: " << screen << std::endl;
        system("cls");

        if (saveMode) {
			HumanInputGame Game;
			Game.setSaveMode(saveMode);
			Game.run(screen);
			// If the game is not running, break the loop
			if (Game.getRunning() == false) {
				if (Game.getWonLevel()) {
					Game.setWonLevel(false);
					continue;
				}
				else {
					break;
				}
			}
        }
/*
		else if (isLoad || isSilent) {
			FileInputGame Game;
			Game.setSilentMode(isSilent);
			Game.run(screen);
			// If the game is not running, break the loop
			if (Game.getRunning() == false) {
				if (Game.getWonLevel()) {
					Game.setWonLevel(false);
					continue;
				}
				else {
					break;
				}
			}
		}*/
    }

}

/*playSpecificBoard
* Play a specific board from the current directory
* Parameters:
* - game: A reference to the Game object
* return value: none
*/
void Menu::playSpecificBoard() {
    system("cls");
    GameConfig::ConsoleCenter::calculate();
    int centerX = GameConfig::ConsoleCenter::getX();
    int centerY = GameConfig::ConsoleCenter::getY();

    if (screens.empty()) {
        gotoxy(centerX, centerY);
        std::cerr << "Not valid .screen files found in the directory.\n";
        return;
    }

    gotoxy(centerX, centerY - 2);
    std::cout << "Available boards:\n";
    for (size_t i = 0; i < screens.size(); ++i) {
        gotoxy(centerX, static_cast<int>(centerY + i - 1));
        std::cout << i + 1 << ". " << screens[i] << std::endl;
    }

    int choice;
    gotoxy(centerX, centerY + 2);
    std::cout << "Enter the number of the board you want to play: ";
    std::cin >> choice;

    if (choice < 1 || choice > static_cast<int>(screens.size())) {
        system("cls");
        gotoxy(centerX, centerY);
        std::cerr << "Invalid choice. Returning to menu...\n";
        return;
    }

    const std::string& selectedBoard = screens[choice - 1];
    gotoxy(centerX, centerY);
    std::cout << "Loading board: " << selectedBoard << std::endl;

    // Start the game with the loaded board
    system("cls");
    HumanInputGame silentGame;
    silentGame.setSaveMode(saveMode);
    silentGame.run(selectedBoard);
}


/*show
* Show the main menu and handle user input
* Parameters:
* - game: A reference to the Game object
* Return value: none
*/
void Menu::show() {
    ShowConsoleCursor(false);
    bool exitMenu = false;

    while (!exitMenu) {

        system("cls");
        GameConfig::ConsoleCenter::calculate();
        int centerX = GameConfig::ConsoleCenter::getX();
        int centerY = GameConfig::ConsoleCenter::getY();

		if (isLoad) {
            gotoxy(centerX, centerY);
			std::cout << "Welcome to Donkey Kong Game (Load Mode)\n";
			Sleep(100);
            system("cls");
            get_all_screens(screens);

            for (const auto& screen : screens) {
				system("cls");
                FileInputGame Game;
				Game.setSilentMode(isSilent);
                Game.run(screen);



	/*			//if no valid steps files found in the directory or is empty or failed to read the file go to the next screen
				if (Game.getSteps().getStepsSize() == 0 ) {
					std::cerr << "No valid steps files found in the directory , moving to the next screen\n";
					//system("cls");
					//exitMenu = true;
                    break;
                }
				else if (Game.getResults().getResultsSize() == 0) {
					std::cerr << "No valid results files found in the directory , moving to the next screen\n";
					//system("cls");
					//exitMenu = true;
                    break;
				}*/
				
                // If the game is not running, break the loop
                if (Game.getRunning() == false) {
                    if (Game.getWonLevel()) {
                        Game.setWonLevel(false);
                        continue;
                    }
                    else {
						system("cls");
                        break;
                    }
                }

            }

		}

        // First line
        gotoxy(centerX, centerY);
        std::cout << "(1) Start a new game\n";

        // Next line: move down one row
        gotoxy(centerX, centerY + 1);
        std::cout << "(8) Present instructions and keys\n";

        // Next line: move down another row
        gotoxy(centerX, centerY + 2);
        std::cout << "(9) EXIT\n";

        char choice1 = _getch();
        switch (choice1) {
        case '1':
            system("cls");
            get_all_screens(screens);
            typeOfGame();
            // After returning from the game, loop automatically shows the menu again
            _getch(); // Wait for user input
            break;
        case '8':
            system("cls");
            gotoxy(GameConfig::ConsoleCenter::getX(), GameConfig::ConsoleCenter::getY());
            presentInstructions();
            _getch(); // Wait for user input
            // After key press, loop continues and shows the menu again
            break;
        case '9':
            system("cls");
            gotoxy(GameConfig::ConsoleCenter::getX(), GameConfig::ConsoleCenter::getY());
            std::cout << "Exiting game...\n";
            exitMenu = true;
            break;
        default:
            std::cout << "Invalid choice. Please select 1, 8, or 9.\n";
            //_getch(); // Pause to let user read the message
            // Just loop back to show the menu again.
            break;
        }
    }
}

/*presentInstructions
* Present the instructions and keys to the user
* The user can press any key to return to the menu
* return value: none
*/
void Menu::presentInstructions() {
    int centerX = GameConfig::ConsoleCenter::getX();
    int centerY = GameConfig::ConsoleCenter::getY();

    // Clear screen or reposition as needed if not done by the caller:
    system("cls");
    gotoxy(centerX, centerY); // Only if you want the first line exactly at centerY.

    gotoxy(centerX, centerY);
    std::cout << "Instructions and keys:\n";

    gotoxy(centerX, centerY + 1);
    std::cout << "LEFT: a or A\n";

    gotoxy(centerX, centerY + 2);
    std::cout << "RIGHT: d or D\n";

    gotoxy(centerX, centerY + 3);
    std::cout << "UP / JUMP: w or W\n";

    gotoxy(centerX, centerY + 4);
    std::cout << "DOWN: x or X\n";

    gotoxy(centerX, centerY + 5);
    std::cout << "STAY: s or S\n";

    gotoxy(centerX, centerY + 6);
    std::cout << "Press any key to return to the menu.\n";
}

/*typeOfGame
* Present the options for the type of game to play
* Parameters:
* - game: A reference to the Game object
* Return value: none
*/
void Menu::typeOfGame() {
    system("cls");
    GameConfig::ConsoleCenter::calculate();
    int centerX = GameConfig::ConsoleCenter::getX();
    int centerY = GameConfig::ConsoleCenter::getY();


    // First line
    gotoxy(centerX, centerY);
    std::cout << "(1) Play All Boards\n";

    // Next line: move down one row
    gotoxy(centerX, centerY + 1);
    std::cout << "(2) Play Specific Board\n";

    // Next line: move down another row
    gotoxy(centerX, centerY + 2);
    std::cout << "(9) EXIT\n";

    char choice1 = _getch();
    switch (choice1) {
    case '1':
        system("cls");
        playAllBoards();
        // After returning from the game, loop automatically shows the menu again
        _getch(); // Wait for user input
        break;
    case '2':
        system("cls");
        playSpecificBoard();
        _getch(); // Wait for user input
        // After key press, loop continues and shows the menu again
        break;
    case '8':
        system("cls");
        playSpecificBoard();
        _getch(); // Wait for user input
        // After key press, loop continues and shows the menu again
        break;
    default:
        std::cout << "Invalid choice. Please select 1, 8, or 9.\n";
        _getch(); // Pause to let user read the message
        // Just loop back to show the menu again.
        break;
    }
}
