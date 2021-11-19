#pragma once
#include "Board.h"
//#include <stdio.h>

// TODO: add protection if someone tries to save game and game is not set up
// TODO: add an option when going back to game if te game was not played then load last save if there is no save load new game

enum class InterfaceState { MENU, NEW_GAME, SAVE, LOAD, OPTIONS, HELP, BACK_TO_GAME, EXIT };

class Interface
{
public:
	Board board1;
	std::vector<std::string> optionData{ 1, "Da\n" };
	std::vector<std::string> loadNames;

	InterfaceState LoadGame(bool resume);
	InterfaceState Load();
	InterfaceState Save();
	InterfaceState LoadOptions();
	InterfaceState LoadHelp();
	InterfaceState MenuIO();
	InterfaceState NewGameIO();
	void Menu();
};