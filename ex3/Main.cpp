#include <iostream>
#include <windows.h>
#include <conio.h>
#include "Menu.h"

int main(int argc, char** argv) {

	bool isLoad = argc > 1 && std::string(argv[1]) == "-load"; // Check if the user wants to load a game
	bool saveMode = argc > 1 && std::string(argv[1]) == "-save"; // Check if the user wants to save the game
	bool isSilent = isLoad && argc > 2 && std::string(argv[2]) == "-silent"; // Check if the user wants to load a game silently

    Menu menu(isLoad , saveMode , isSilent);
    menu.show();

	return 0;
}