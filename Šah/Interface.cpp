#include "Interface.h"

InterfaceState Interface::LoadGame(bool resume)
{
	Gamestate gamestate = Gamestate::SELECT;
	Board::Target id;
	int a = 32;
	bool reSelect = false;

	system("cls");
	if (resume == false)
	{
		board1.SetBoard();
		board1.playerTurn = true;
	}
	board1.PrintBoard();
	if (board1.playerTurn == true)
		std::cout << "Crveni igrac je na redu.\n";
	else std::cout << "Plavi igrac je na redu.\n";

	while (gamestate != Gamestate::EXIT)
	{
		switch (gamestate)
		{
		case Gamestate::SELECT:
			gamestate = board1.SelectFigure(reSelect, &id);
			break;
		case Gamestate::MOVE:
			gamestate = board1.MoveFigure(reSelect, &id);
			if(reSelect == false)
			{
				if (board1.playerTurn == true)
					board1.playerTurn = false;
				else
					board1.playerTurn = true;
			}
			else
				reSelect = false;
			break;
		}
		
		if (board1.playerTurn == true)
			std::cout << "Crveni igrac je na redu.\n";
		else std::cout << "Plavi igrac je na redu.\n";
	}
	return InterfaceState::MENU;
}

// TODO: improve this function
InterfaceState Interface::Load()
{
	std::string input;
	std::vector<std::string> loadNames;
	system("cls");
	std::cout << "Spremili ste ove igre:\n\n";
	if (board1.LoadGameData(loadNames) == true)
	{
		for (int i = 2; i < loadNames.size(); i++)
		{
			for (int j = 0; j < loadNames[i].size() - 4; j++)
			{
				std::cout << loadNames[i][j];
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	// TODO: add help what delete(ime) does
	std::cout << "Upisite ime:\n";
	std::cout << "Upisite delete(ime): \n";
	std::cout << "Natrag(x):";
	std::cin >> input;
	if (input.front() == 'x' && input.size() == 1)
		Menu();
	else if (input[0] == 'd' && input[1] == 'e' && input[2] == 'l' && input[3] == 'e' && input[4] == 't' && input[5] == 'e' 
		&& input[6] == '(')
	{
		std::string partInput;
		bool deleted = false;
		for (int i = 7; i < input.size()-1; i++)
			partInput.push_back(input[i]);
		partInput.push_back('.');
		partInput.push_back('t');
		partInput.push_back('x');
		partInput.push_back('t');
		
		for (int i = 2; i < loadNames.size(); i++)
		{
			if (partInput == loadNames[i])
			{
				loadNames[i] = loadNames.back();
				loadNames.pop_back();
				board1.SaveGameData(loadNames);
				std::cout << "Uspješno izbrisano.\n";
				deleted = true;
				break;
			}
		}
		if (deleted == true)
		{
			std::cout << "Uspješno izbrisano.\n";
			deleted = false;
		}
		else
			std::cout << "Greska!\n";
		return InterfaceState::LOAD;
	}
	else
	{
		input.push_back('.');
		input.push_back('t');
		input.push_back('x');
		input.push_back('t');
		if (board1.LoadGame(input) == false)
		{
			std::cout << "Ucitavanje nije uspjelo!\n";
			std::cout << "Stisnite bilo koju tipku za natrag.\n";
			std::cin >> input;
		}
		else
			return InterfaceState::BACK_TO_GAME;
	}
	return InterfaceState::MENU;
}

InterfaceState Interface::Save()
{
	// TODO: add a list 1234 like in menu input
	std::string input;
	bool haveName = false;
	bool overwrite = true;
	system("cls");
	std::cout << "Spremili ste ove igre:\n\n";
	if (board1.LoadGameData(loadNames) == true)
	{
		for (int i = 2; i < loadNames.size(); i++)
		{
			for (int j = 0; j < loadNames[i].size() - 4; j++)
			{
				std::cout << loadNames[i][j];
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	std::cout << "Upisite ime:\n";
	std::cout << "Natrag(x):";
	std::cin >> input;
	if (input.front() == 'x' && input.size() == 1)
		return InterfaceState::MENU;
	else
	{
		input.push_back('.');
		input.push_back('t');
		input.push_back('x');
		input.push_back('t');
		for (int i = 2; i < loadNames.size(); i++)
		{
			if (loadNames[i] == input)
			{
				char ch;
				std::cout << "Pisati preko " << input << " (d/n)?\n";
				scanf_s(" %c", &ch);
				if (ch != 'd' && ch != 'n')
					board1.bad = true;
				while (board1.bad == true)
				{
					std::cout << "Nevazeci ulaz!\n";
					scanf_s(" %c", &ch);
					if (ch == 'd' || ch == 'n')
						board1.bad = false;
				}
				if (ch == 'n')
					overwrite = false;
				break;
			}
		}
		if (overwrite == true)
		{
			if (board1.SaveGame(input) == false)
			{
				std::cout << "Spremanje nije uspjelo!\n";
				std::cout << "Stisnite bilo koju tipku za natrag.\n";
				std::cin >> input;
				return InterfaceState::MENU;
			}
			else
			{
				if (board1.LoadGameData(loadNames) == true)
				{
					for (int i = 0; i < loadNames.size(); i++)
					{
						if (loadNames[i] == input)
						{
							haveName = true;
							break;
						}

					}
					if (haveName == false)
						loadNames.push_back(input);
					else
						haveName = false;
				}
				else
				{
					loadNames.push_back(optionData.front());
					loadNames.push_back(input);
				}
				//first you need to load game data push back input then save it
				board1.SaveGameData(loadNames);
				std::cout << "Spremanje je uspjelo!\n";
				std::cout << "Stisnite bilo koju tipku za natrag.\n";
				std::cin >> input;
			}
		}
		else
			Save();
	}
	return InterfaceState::MENU;
}
InterfaceState Interface::LoadOptions()
{
	char input;
	system("cls");
	if (board1.LoadGameData(loadNames) == true)
	{
		if (loadNames.front() == "Da\n")
			board1.showAvalibleSpots = true;
		else
			board1.showAvalibleSpots = false;
	}
	std::cout << "1...Prikazi slobodna mjesta: ";
	if (board1.showAvalibleSpots == true)
		std::cout << "Da\n";
	else
		std::cout << "Ne\n";
	std::cout << "\n2...Natrag:\n";
	scanf_s(" %c", &input);
	if (input > '0' && input < '3')
		;
	else
		board1.bad = true;
	while (board1.bad == true)
	{
		printf("Nevazeci ulaz!\n");
		scanf_s(" %c", &input);
		if (input > '0' && input < '3')
			board1.bad = false;
		else
			board1.bad = true;
	}
	if (input == '1')
	{
		if (board1.showAvalibleSpots == true)
		{
			board1.showAvalibleSpots = false;
			optionData.front() = "Ne\n";
		}
		else
		{
			board1.showAvalibleSpots = true;
			optionData.front() = "Da\n";
		}
		if (board1.LoadGameData(loadNames) == true)
		{
			if (board1.showAvalibleSpots == true)
				loadNames.front() = "Da\n";
			else
				loadNames.front() = "Ne\n";
			board1.SaveGameData(loadNames);
		}
		else
		{
			if (board1.showAvalibleSpots == true)
				loadNames.push_back("Da\n");
			else
				loadNames.push_back("Ne\n");
		}	
		return InterfaceState::OPTIONS;
	}
	return InterfaceState::MENU;
}
InterfaceState Interface::LoadHelp()
{
	// in help
	char input;
	system("cls");
	std::cout << "\n(1)Potrebna rezolucija konzole 8x8!\n";
	std::cout << "Stisnite na ikonu programa(gore lijevo od prozora) -> Properties -> Font -> Size -> 8x8\n";
	std::cout << "\n(2)Ako ne mozete promijeniti velistd::cinu prozora.\n";
	std::cout << "Stisnite na ikonu programa(gore lijevo od prozora) -> Properties -> Layout -> Window size -> Width: 140, Height: 96\n";
	std::cout << "\n(3)Kako igrati?\n";
	std::cout << "Ukucate poziciju figure koju zelite pomaknuti 'a2' i ukucate poziciju gdje zelite da bude.\n";
	std::cout <<	"Ako zelite drugi figuru uzeti ukucate 'r'.\n";
	
	std::cout << "\n1...Kako igrati sah?\n";
	std::cout << "2...Natrag:\n";
	scanf_s(" %c", &input);
	if (input > '0' && input < '3')
		;
	else
		board1.bad = true;
	while (board1.bad == true)
	{
		printf("Nevazeci ulaz!\n");
		scanf_s(" %c", &input);
		if (input > '0' && input < '3')
			board1.bad = false;
		else
			board1.bad = true;
	}
	if (input == '1')
	{
		system("start www.chess.com/learn-how-to-play-chess");
		return InterfaceState::HELP;
	}
	return InterfaceState::MENU;
}

InterfaceState Interface::MenuIO()
{
	char input;
	InterfaceState returnType;
	system("cls");
	std::cout << "Dobro dosli u sah!\n\n\n";
	std::cout << "1...Nova igra:\n\n";
	std::cout << "2...Spremi:\n\n";
	std::cout << "3...Ucitaj:\n\n";
	std::cout << "4...Opcije:\n\n";
	std::cout << "5...Pomoc:\n\n";
	std::cout << "6...Natrag u igru:\n\n";
	std::cout << "7...Izlaz:\n";

	scanf_s(" %c", &input);
	if (input > '0' && input < '8')
		;
	else
		board1.bad = true;
	while (board1.bad == true)
	{
		printf("Nevazeci ulaz!\n");
		scanf_s(" %c", &input);
		if (input > '0' && input < '8')
			board1.bad = false;
		else
			board1.bad = true;
	}
	switch (input)
	{
	case'1': returnType = InterfaceState::NEW_GAME;
		break;
	case'2': returnType = InterfaceState::SAVE;
		break;
	case'3': returnType = InterfaceState::LOAD;
		break;
	case'4': returnType = InterfaceState::OPTIONS;
		break;
	case'5': returnType = InterfaceState::HELP;
		break;
	case'6': returnType = InterfaceState::BACK_TO_GAME;
		break;
	case '7': returnType = InterfaceState::EXIT;
		break;
	}
	return returnType;
}

InterfaceState Interface::NewGameIO()
{
	InterfaceState returnType;
	if (board1.IsGamePlayed() == true)
	{// PERROR: if game has ended you need to clear moves othervise it will ask
		char ch;
		std::cout << "Izbrisati staru igru(d/n)?\n";
		scanf_s(" %c", &ch);
		if (ch != 'd' && ch != 'n')
			board1.bad = true;
		while (board1.bad == true)
		{
			std::cout << "Nevazeci ulaz!\n";
			scanf_s(" %c", &ch);
			if (ch == 'd' || ch == 'n')
				board1.bad = false;
		}
		if (ch == 'n')
			returnType = InterfaceState::SAVE;
		else
			returnType = LoadGame(false);
	}
	else
		returnType = LoadGame(false);
	return returnType;
}

// use scanf_s(" %c", &input); with space before format to get rid of \n in buffer
void Interface::Menu()
{
	// TODO: add a vector of strings to output to screen
	InterfaceState menuState = InterfaceState::MENU;
	while (menuState != InterfaceState::EXIT)
	{
		switch (menuState)
		{
		case InterfaceState::MENU:
			menuState = MenuIO();
			break;
		case InterfaceState::NEW_GAME:
			menuState = NewGameIO();
			break;
		case InterfaceState::SAVE:
			menuState = Save();
			break;
		case InterfaceState::LOAD:
			menuState = Load();
			break;
		case InterfaceState::OPTIONS:
			menuState = LoadOptions();
			break;
		case InterfaceState::HELP:
			menuState = LoadHelp();
			break;
		case InterfaceState::BACK_TO_GAME:
			menuState = LoadGame(true);
			break;
		}
	}
}
