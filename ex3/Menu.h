/* ------------------------- MENU ---------------------------------------
 * The Menu class provides functionalities to display different screens, manage user interactions, and control game flow.
 */
#pragma once
#include <iostream>
#include "gameConfig.h"
#include "FileInputGame.h"
#include "HumanInputGame.h"

class Menu {
private:
	bool isLoad;
	bool saveMode;
	bool isSilent;
	vector<string> screens{};

public:
	Menu(bool isLoad, bool saveMode, bool isSilent) : isLoad(isLoad), saveMode(saveMode), isSilent(isSilent) {}
	void show();
	void presentInstructions();
	void typeOfGame();
	void get_all_screens(vector<string>& screens);
	void playAllBoards();
	void playSpecificBoard();
};
