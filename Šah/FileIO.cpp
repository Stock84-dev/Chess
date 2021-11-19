#include "Board.h"
#include <fstream>

//********************************************************************************************************//
//*********************************************File streams***********************************************//
//********************************************************************************************************//

bool Board::SaveGame(std::string input)
{
	std::ofstream save;

	save.open(input);
	if (save.fail())
		return false;

	for (int j = 0; j < figure.size(); j++)
	{
		for (int i = 0; i < figure[j].size(); i++)
		{
			if (figure[j][i].pos > 9)
				save << figure[j][i].name << " " << figure[j][i].pos << " " << figure[j][i].moves << std::endl;
			else
				save << figure[j][i].name << " " << figure[j][i].pos << "  " << figure[j][i].moves << std::endl;
		}
		save << std::endl;
	}
	for (int j = 0; j < 2; j++)
		save << eatenFigures[j].top << " " << eatenFigures[j].konj << " " << eatenFigures[j].lovac << " " << eatenFigures[j].kraljica << " " << eatenFigures[j].pijun << std::endl;

	if (playerTurn == true)
		save << "1";
	else
		save << "0";
	save.close();
	return true;
}

bool Board::LoadGame(std::string input)
{
	std::ifstream load;
	Figure figure1, figure2;
	std::string line;
	std::vector<Figure> tmp;
	int pos, breakpoint = 0;

	load.open(input);
	if (load.fail())
		return false;
	if (figure.size() != 0)
	{
		figure[0].clear();
		figure[1].clear();
	}
	else
	{
		figure.push_back(tmp);
		figure.push_back(tmp);
	}
	while (getline(load, line))
	{
		if (line.size() < 14)
			breakpoint++;

		for (int i = 0; i < line.size(); i++)
		{
			if (breakpoint < 3)
			{
				if (i < 9)
					figure1.name.push_back(line[i]);
				if (i == 9)
					figure1.name[i] = '\0';
				else if (i == 10)
				{
					if (line[i + 1] != ' ')
						figure1.pos = (line[i] - 48) * 10 + line[i + 1] - 48;
					else
						figure1.pos = line[i] - 48;
				}
				else if (i == 13)
					figure1.moves = line[i] - 48;
			}
			else if (breakpoint == 3)
			{
				eatenFigures[0].top = line[0] - 48;
				eatenFigures[0].konj = line[2] - 48;
				eatenFigures[0].lovac = line[4] - 48;
				eatenFigures[0].kraljica = line[6] - 48;
				eatenFigures[0].pijun = line[8] - 48;
			}
			else if (breakpoint == 4)
			{
				eatenFigures[1].top = line[0] - 48;
				eatenFigures[1].konj = line[2] - 48;
				eatenFigures[1].lovac = line[4] - 48;
				eatenFigures[1].kraljica = line[6] - 48;
				eatenFigures[1].pijun = line[8] - 48;
			}
			else if (breakpoint == 5)
			{
				if (line.front() == '0')
					playerTurn = false;
				else
					playerTurn = true;
			}
		}

		if (breakpoint == 0)
			figure[0].push_back(figure1);
		else if (breakpoint == 1)
			figure[1].push_back(figure1);

		figure1 = figure2;
	}
	return true;
}

//you need to save saving names load names options
bool Board::SaveGameData(std::vector<std::string> &loadNames)
{
	std::ofstream saveData;
	saveData.open("SahGameData.txt");
	if (saveData.fail())
		return false;

	for (int i = 0; i < loadNames.size(); i++)
	{
		saveData << loadNames[i] << std::endl;
	}
	saveData.close();
	return true;
}

bool Board::LoadGameData(std::vector<std::string> &loadNames)
{
	std::ifstream loadData;
	std::string input;
	bool haveName = false;
	loadData.open("SahGameData.txt");
	if (loadData.fail())
		return false;

	while (getline(loadData, input))
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
	loadData.close();
	return true;
}