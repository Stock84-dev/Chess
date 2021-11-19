#include "Board.h"
#include <iostream>

//*******************************************************************************************************************//
//*********************************************Testing for move functions********************************************//
//*******************************************************************************************************************//

void Board::PrintPossiblePaths(int pos, int colorId)
{
	Target id0;
	//GetUserCoordinates(figure[targetFigure[0]][targetFigure[1]].pos - i);
	ConvertPosToCoordinate(pos);
	SetCursorPosition(coordinate[0] - 4, coordinate[1] - 4);
	if (colorMap[pos] < 240)
	{//dimmer green beneath names
		if (colorId == 0)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 41);//red
		else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 44);//blue
	}
	else
	{//brighter green beneath names
		if (colorId == 0)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 169);
		else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 172);
	}

	for (int j = 1; j < 10; j++)
	{
		if (j == 5)
		{//printing other figures name
			if (SearchFigure(pos, &id0) == true)
				std::cout << figure[id0.color][id0.type].name;
			else
				std::cout << "         ";
		}
		else
			std::cout << "         ";
		SetCursorPosition(coordinate[0] - 4, coordinate[1] - 4 + j);
	}
}

void Board::RemovePossiblePaths()
{
	Target id;
	for (int i = 0; i < possiblePaths.size(); i++)
	{
		if (possiblePaths[i] == 0)
			break;
		ConvertPosToCoordinate(possiblePaths[i]);
		SetCursorPosition(coordinate[0] - 4, coordinate[1] - 4);
		ChangeColor(possiblePaths[i]);
		for (int j = 1; j < 10; j++)
		{
			if (j == 5)
			{
				if (SearchFigure(possiblePaths[i], &id) == true)
					std::cout << figure[id.color][id.type].name;
				else
					std::cout << "         ";
			}
			else
				std::cout << "         ";
			SetCursorPosition(coordinate[0] - 4, coordinate[1] - 4 + j);
		}
	}
}

void Board::PawnMove(int targetPos, int forceSearch, bool show, Target id)
{
	int j, i, pos;
	if (id.color == 0)
	{
		i = 7;
		j = 2;
	}
	else
	{
		i = -7;
		j = -2;
	}
	//eating
	for (; i > -10 && i < 10; i += j)
	{
		if (((i == 7 || i == -9) && (targetPos - 1) % 8 == 0) || ((i == 9 || i == -7) && targetPos % 8 == 0))
			continue;
		pos = targetPos + i;
		if ((TestForFigure(pos, false, false) == true))
		{
			if (show == true)
				PrintPossiblePaths(pos, id.color);
			possiblePaths.push_back(pos);
		}
	}
	if (forceSearch != 6)
	{
		if (id.color == 0)
			i = 8;
		else
			i = -8;
		//moving
		if (figure[id.color][id.type].moves != 0)
		{
			pos = targetPos + i;
			if (TestForFigure(pos, false, true) == false)
			{
				if (show == true)
					PrintPossiblePaths(pos, id.color);
				possiblePaths.push_back(pos);
			}
		}
		//first move
		else
			for (; i > -17 && i < 17; i += i)
			{
				pos = targetPos + i;
				if (TestForFigure(pos, false, true) == false)
				{
					if (show == true)
						PrintPossiblePaths(pos, id.color);
					possiblePaths.push_back(pos);
				}
				else
					break;
			}
	}
}
// FIXME: test it in console if ignorePos is at border to not go beyond border
void Board::TowerMove(int targetPos, int forceSearch, bool show, int ignorePos, Target id)
{
	int type = 1, i = -1, pos, tmp = 1;
	while (type < 5)
	{
		//if we have changed type we need to set i
		if (type != tmp)
		{
			switch (type)
			{
			case 2: i = 1;
				break;
			case 3: i = 8;
				break;
			case 4: i = -8;
			}
		}
		tmp = type;
		pos = targetPos + i;

		//if we are over border 
		if (((pos - 1) % 8 == 0 && type == 2) || (pos % 8 == 0 && type == 1) || (pos > 64 && type == 3) || (pos < 1 && type == 4)
			|| TestForFigure(pos, true))
		{
			type++;
			continue;
		}

		//usualy this will be better to put that at end but no duplications
		switch (type)
		{
		case 1: i--;
			break;
		case 2: i++;
			break;
		case 3: i += 8;
			break;
		case 4: i -= 8;
		}

		//skipping specific pos
		if (/*(forceSearch == 1 && figure[selectedFigure[0]][selectedFigure[1]].name[5] == 'l' &&
			figure[selectedFigure[0]][selectedFigure[1]].pos == pos) ||*/ ignorePos == pos)
			continue;

		possiblePaths.push_back(pos);//keeping track of paths to remove it
		if (show == true)
			PrintPossiblePaths(pos, id.color);

		//if there was enemy
		if (TestForFigure(pos, false))
			type++;
	}
}

void Board::HorseMove(int targetPos, int forceSearch, bool show, Target id)
{
	int type = 0, i, pos;
	for (int j = 0; j < 8; j++)
	{
		if (j % 2 == 0)
		{
			type++;
			/*if (type == 5)
			break;*/
			switch (type)
			{
			case 1: i = 6;
				break;
			case 2: i = 10;
				break;
			case 3: i = 17;
				break;
			case 4: i = -15;
			}
		}
		//all sides that are going out of map
		if ((type == 1 && ((targetPos - 2) % 8 == 0 || (targetPos - 1) % 8 == 0)) || (type == 3 && targetPos > 48) ||
			(type == 2 && ((targetPos + 1) % 8 == 0 || targetPos % 8 == 0)) || (type == 4 && targetPos < 17))
		{
			j++;
			continue;
		}
		//left/right up side
		else if ((type == 1 || type == 2) && targetPos > 56 && (i == 6 || i == 10))
		{
			i -= 16;
			continue;
		}
		//left/right down side
		else if ((type == 1 || type == 2) && targetPos < 9 && (i == -6 || i == -10))
			continue;

		//up/down right side
		else if ((type == 3 || type == 4) && targetPos % 8 == 0 && (i == -15 || i == 17))
		{
			i -= 2;
			continue;
		}
		//up/down left side
		else if ((type == 3 || type == 4) && (targetPos - 1) % 8 == 0 && (i == 15 || i == -17))
		{
			if (type == 4)
				break;
			else
				continue;
		}
		pos = targetPos + i;

		if (pos > 0 && pos < 65 && (TestForFigure(pos, false, false) == true || TestForFigure(pos, false, true) == false))
		{
			if (show == true)
				PrintPossiblePaths(pos, id.color);
			possiblePaths.push_back(pos);
		}
		if (type == 1 || type == 2)
			i -= 16;
		else i -= 2;
	}
}

void Board::HunterMove(int targetPos, int forceSearch, bool show, int ignorePos, Target id)
{
	int i = 7, tmp = 1, type = 1, pos;
	while (type < 5)
	{
		if (tmp != type)
		{
			switch (type)
			{
			case 2: i = 9;
				break;
			case 3: i = -9;
				break;
			case 4: i = -7;
				break;
			default: continue;
			}
		}
		tmp = type;
		if ((targetPos > 56 && type < 3) || (targetPos < 9 && type > 2) || (targetPos % 8 == 0 && (type == 2 || type == 4)) ||
			((targetPos - 1) % 8 == 0 && (type == 1 || type == 3)))
		{
			type++;
			continue;
		}
		pos = targetPos + i;
		if (/*(forceSearch == 3 && figure[selectedFigure[0]][selectedFigure[1]].name[5] == 'l' &&
			figure[selectedFigure[0]][selectedFigure[1]].pos == pos) ||*/ ignorePos == pos)
		{
			if (type == 1)
				i += 7;
			else if (type == 2)
				i += 9;
			else if (type == 3)
				i += -9;
			else if (type == 4)
				i += -7;
			continue;
		}
		if (pos > 0 && pos < 65 && (TestForFigure(pos, false, true) == false || TestForFigure(pos, false, false) == true))
		{
			if (show == true)
				PrintPossiblePaths(pos, id.color);
			possiblePaths.push_back(pos);
			if (TestForFigure(pos, false, false) == true)
			{
				type++;
				continue;
			}
		}
		else
		{
			type++;
			continue;
		}
		if (pos % 8 == 0 || (pos - 1) % 8 == 0 || pos < 9 || pos > 56)
		{
			type++;
			continue;
		}
		switch (type)
		{
		case 1: i += 7;
			break;
		case 2: i += 9;
			break;
		case 3: i -= 9;
			break;
		case 4: i -= 7;
		}
	}
}

void Board::KingMove(int targetPos, int forceSearch, bool show, Target id)
{
	bool mine = false;
	Target id0;
	std::vector<int> kingPaths;
	int i = -10, pos;
	/*if (forceSearch != 5)
	{
	selectedFigure[0] = id.color;
	selectedFigure[1] = id.type;
	}*/
	while (true)
	{
		if (i == -7 || i == 1)
			i += 6;
		else if (i == -1)
			i += 2;
		else
		{
			i++;
			if (i > 9)
				break;
		}
		pos = targetPos + i;
		//if figure is on border skip
		if ((i < -1 && figure[id.color][id.type].pos < 9) ||
			((i == -9 || i == -1 || i == 7) && (figure[id.color][id.type].pos - 1) % 8 == 0) ||
			((i == -7 || i == 1 || i == 9) && figure[id.color][id.type].pos % 8 == 0) ||
			(i > 1 && i < 10 && figure[id.color][id.type].pos > 56))
			continue;

		if (TestForFigure(pos, false, false) == true || TestForFigure(pos, false, true) == false)
		{
			if (forceSearch == 5)
				possiblePaths.push_back(pos);
			else
			{
				if (TestForCheckAtPos(pos, playerTurn, 0) == false)
					kingPaths.push_back(pos);
			}
		}
	}
	
	if (forceSearch != 5)
	{
		for (int j = 0; j < kingPaths.size(); j++)
		{
			if (show == true)
				PrintPossiblePaths(kingPaths[j], id.color);
		}
		possiblePaths.swap(kingPaths);
	}
}

/*
void Board::KingMove(int targetPos, int forceSearch, bool show, Target id)
{
bool mine = false;
Target id0;
std::vector<int> kingPaths;
int i = -10, pos;
while (true)
{
	if (i == -7 || i == 1)
		i += 6;
	else if (i == -1)
		i += 2;
	else
	{
		i++;
		if (i > 9)
			break;
	}
	pos = targetPos + i;
	//if figure is on border skip
	if ((i < -1 && figure[id.color][id.type].pos < 9) ||
		((i == -9 || i == -1 || i == 7) && (figure[id.color][id.type].pos - 1) % 8 == 0) ||
		((i == -7 || i == 1 || i == 9) && figure[id.color][id.type].pos % 8 == 0) ||
		(i > 1 && i < 10 && figure[id.color][id.type].pos > 56))
		continue;
	if (forceSearch == 5)
	{
		if ((TestForFigure(pos, false, false) == true || TestForFigure(pos, false, true) == false))
			possiblePaths.push_back(pos);
	}
	//if there is free spot or enemy figure
	if ((TestForFigure(pos, false, false) == true || TestForFigure(pos, false, true) == false) && forceSearch != 5)
	{//Changes figure type
		for (int l = 1; l < 7; l++)
		{//performs search with every figure type on king's avalible spots if it finds figure that matches search places mine
			if (l == 4)//You don't need to check for queen
				continue;
			TestForMove(pos, l, false, false, 0, id);
			for (int k = 0; k < possiblePaths.size(); k++)
			{
				if (SearchFigure(possiblePaths[k], &id0) == true)
				{
					if (((figure[id0.color][id0.type].name[5] == 'p' || figure[id0.color][id0.type].name[5] == 'i') && l == 1) ||//Top
						(figure[id0.color][id0.type].name[5] == 'j' && l == 2) ||//Konj
						((figure[id0.color][id0.type].name[5] == 'a' || figure[id0.color][id0.type].name[5] == 'i') && l == 3) ||//Lovac
						(figure[id0.color][id0.type].name[5] == 'l' && l == 5) ||//Kralj
						(figure[id0.color][id0.type].name[5] == 'u' && l == 6))  //Pijun
					{
						mine = true;
						l = 7;
						break;
					}
				}
			}
			//RemovePossiblePaths();
		}
		if (mine == false)
		{
			kingPaths.push_back(pos);
			if (show == true)
				PrintPossiblePaths(pos, id.color);
		}
		else
			mine = false;
	}
}
if (forceSearch != 5)
possiblePaths.swap(kingPaths);

// TODO: improve check and checkmate or add
}
*/

void Board::TestForMove(int targetPos, int forceSearch, bool show, bool stackPaths, int ignorePos, Target id)
{
	if (stackPaths == false)
		possiblePaths.clear();// PERROR: it is not guaranteed that a vector will realocate its size to 0

	//Pijun
	if (figure[id.color][id.type].name[5] == 'u' || forceSearch == 6)
		PawnMove(targetPos, forceSearch, show, id);

	//Top
	else if (figure[id.color][id.type].name[5] == 'p' || forceSearch == 1)
		TowerMove(targetPos, forceSearch, show, ignorePos, id);

	//Konj
	else if (figure[id.color][id.type].name[5] == 'j' || forceSearch == 2)
		HorseMove(targetPos, forceSearch, show, id);

	//Lovac
	else if (figure[id.color][id.type].name[5] == 'a' || forceSearch == 3)
		HunterMove(targetPos, forceSearch, show, ignorePos, id);

	//Kraljica
	else if (figure[id.color][id.type].name[5] == 'i' || forceSearch == 4)
	{
		/*selectedFigure[0] = 0;
		selectedFigure[1] = 0;*/
		TestForMove(figure[id.color][id.type].pos, 1, true, false, 0, id);
		TestForMove(figure[id.color][id.type].pos, 3, true, true, 0, id);
	}

	//Kralj	TODO
	else if (figure[id.color][id.type].name[5] == 'l' || forceSearch == 5)
		KingMove(targetPos, forceSearch, show, id);

	ClearChat();
}

// FIXME: don't use king pos to to ignore better use ignore pos in king functions
// TODO: improve check ai for now ignore it 
bool Board::TestForCheck(int pos, Target id)
{
	int kingPos;
	Target id0;

	//selectedFigure[0] = 0;
	//selectedFigure[1] = 0;
	//whenever a player selects a figure test for check by ignoring that pos for ally king if there is no check let player move figure 
	//else output you can't move figure it will be chess please pick another figure
	if (playerTurn == true)//Red
		kingPos = figure[0][1].pos;
	else
		kingPos = figure[1][1].pos;
	//Changes figure type
	for (int l = 1; l < 4; l += 2)
	{
		TestForMove(kingPos, l, false, false, pos, id);
		for (int k = 0; k < possiblePaths.size(); k++)
		{
			if (SearchFigure(possiblePaths[k], &id0) == true)
			{
				if (((figure[id0.color][id0.type].name[5] == 'p' || figure[id0.color][id0.type].name[5] == 'i') && l == 1) ||//Top
					((figure[id0.color][id0.type].name[5] == 'a' || figure[id0.color][id0.type].name[5] == 'i') && l == 3))//Lovac
					return true;
			}
		}
	}
	return false;
}
//TODO: you can optimize this to save only last possibble path avalible when searching 
bool Board::TestForCheckAtPos(int pos, bool AllyColor, int ignorePos)
{
	Target id0;
	Target id;
	int kingPos;
	
	if (AllyColor == true)
	{
		id0.color = 0;
		kingPos = figure[0][0].pos;
	}	
	else
	{
		id0.color = 1;
		kingPos = figure[1][0].pos;
	}
	if (ignorePos == 0)
		ignorePos = kingPos;
	id0.type = 0;// FIXME: error
	
	for (int l = 1; l < 7; l++)
	{
		if (l == 4)
			continue;
		TestForMove(pos, l, false, false, ignorePos, id0);
		for (int i = 0; i < possiblePaths.size(); i++)
		{
			if (SearchFigure(possiblePaths[i], &id))
			{
				if (l == 1 && (figure[id.color][id.type].name[5] == 'p' || figure[id.color][id.type].name[5] == 'i'))
					return true;
				else if (l == 2 && figure[id.color][id.type].name[5] == 'j')
					return true;
				else if (l == 3 && (figure[id.color][id.type].name[5] == 'a' || figure[id.color][id.type].name[5] == 'i'))
					return true;
				else if (l == 5 && figure[id.color][id.type].name[5] == 'l')
					return true;
				else if (l == 6 && figure[id.color][id.type].name[5] == 'u')
					return true;
			}
		}
	}
	return false;
}

//***********************************************************************************************************************//
//*********************************************Searching for figure functions********************************************//
//***********************************************************************************************************************//

bool Board::SearchFigure(int pos)
{
	for (int j = 0; j < figure.size(); j++)
	{
		for (int i = 0; i < figure[j].size(); i++)
		{
			if (figure[j][i].pos == pos)
				return true;
		}
	}
	return false;
}

bool Board::SearchFigure(int pos, Target* ID)
{
	for (int j = 0; j < figure.size(); j++)
	{
		for (int i = 0; i < figure[j].size(); i++)
		{
			if (figure[j][i].pos == pos)
			{
				ID->color = j;
				ID->type = i;
				return true;
			}
		}
	}
	return false;
}

bool Board::TestForFigure(int pos, bool ownFigure, bool ignore)
{
	//You can improve this by not searching again for figure
	bool figureExists = false;

	if (ignore == true)
	{
		for (int j = 0; j < figure.size(); j++)
		{
			for (int i = 0; i < figure[j].size(); i++)
			{
				if (figure[j][i].pos == pos)
				{
					figureExists = true;
					j = 2;
					break;
				}
			}
		}
		return figureExists;
	}

	for (int j = 0; j < figure.size(); j++)
	{
		for (int i = 0; i < figure[j].size(); i++)
		{
			if (figure[j][i].pos == pos)
			{
				if (playerTurn == true)
					if (ownFigure == true)
						if (j == 0)
							figureExists = true;
						else figureExists = false;
					else
						if (j == 1)
							figureExists = true;
						else figureExists = false;
				else
					if (ownFigure == true)
						if (j == 1)
							figureExists = true;
						else figureExists = false;
					else
						if (j == 0)
							figureExists = true;
						else figureExists = false;
						j = 2;
						break;
			}
		}
	}
	return figureExists;
}

//**********************************************************************************************************************//
//*********************************************Manipulating figure functions********************************************//
//**********************************************************************************************************************//

bool Board::TestForCastling(int color, int type)
{
	Target id0, id1, kingId;
	int pos = figure[color][type].pos, left = 0, right = 0;
	char in;
	bool returntype = false;

	kingId.color = color;
	kingId.type = type;

	//first we are going to right then to left
	for (int i = 1, repeat = 0, direction = 1; i < 9; i++)
	{
		if (i == 5)
		{
			repeat = 0;
			pos = figure[color][type].pos;
			direction = -1;
			//target1 is right, target0 is left
			id1 = id0;
		}
		pos += 1 * direction;
		//if there is no figure and there is no check
		if (SearchFigure(pos) == false /* FIXME: needs to check for check "&& TestForCheck(pos) == false"*/)
			repeat++;
		else if (repeat == 2 && i < 4 || repeat == 3 && i > 4)//if we are on crook
		{
			if (SearchFigure(pos, &id0))
			{
				if (figure[id0.color][id0.type].moves == 0)
				{
					if (repeat == 2)
						right = 1;
					else left = 1;
				}
			}
		}
		else if (i < 4)
			continue;
		else break;
	}

	if (left == 1 || right == 1)
	{
		std::cout << "Zelite li napraviti rosadu:(";
		if (left == 1)
			std::cout << "l/";
		if (right == 1)
			std::cout << "d/";
		std::cout << "x)\n";

		scanf_s(" %c", &in);
		if (in == 'l' && left == 1 || in == 'd' && right == 1 || in == 'x')
			;
		else bad = true;
		while (bad)
		{
			std::cout << "Nevazeci ulaz!\n";
			scanf_s(" %c", &in);
			if (in == 'l' && left == 1 || in == 'd' && right == 1 || in == 'x')
				bad = false;
		}

		if (in == 'd')
		{
			PrintBeforeMove(id1);
			PrintBeforeMove(kingId);
			figure[kingId.color][kingId.type].pos += 2;
			figure[id1.color][id1.type].pos -= 2;
			PrintAfterMove(id1);
			PrintAfterMove(kingId);
			returntype = true;
		}
		else if (in == 'l')
		{
			PrintBeforeMove(id0);
			PrintBeforeMove(kingId);
			figure[kingId.color][kingId.type].pos -= 2;
			figure[id0.color][id0.type].pos += 3;
			PrintAfterMove(id0);
			PrintAfterMove(kingId);
			returntype = true;
		}
	}
	ClearChat();
	return returntype;
}
// TODO: add target
void Board::Promotion(int color, int type)
{
	char input;

	ClearChat();
	std::cout << "Unaprijedite pijuna:\n";
	std::cout << "1...Top:\n";
	std::cout << "2...Konj\n";
	std::cout << "3...Lovac\n";
	std::cout << "4...Kraljica\n";

	// TODO: maybe make a function that handles all bad input?
	scanf_s(" %c", &input);
	if (input < '1' || input > '4')
		bad = true;
	while (bad)
	{
		std::cout << "Nevazeci ulaz!\n";
		scanf_s(" %c", &input);
		if (input > '0' && input < '5')
			bad = false;
	}
	switch (input)
	{
	case '1': figure[color][type].name = "   Top   ";
		break;
	case '2': figure[color][type].name = "  Konj   ";
		break;
	case '3': figure[color][type].name = "  Lovac  ";
		break;
	case '4': figure[color][type].name = "Kraljica ";
		break;
	}
}

void Board::PrintBeforeMove(Target target)
{
	//Removing figure name from last pos
	ConvertPosToCoordinate(figure[target.color][target.type].pos);
	SetCursorPosition(coordinate[0] - 4, coordinate[1]);
	ChangeColor(figure[target.color][target.type].pos);
	for (int a = 0; a < 8; a++)
		std::cout << ' ';
}

void Board::PrintAfterMove(Target target)
{
	//Printing figure name at new pos
	ConvertPosToCoordinate(figure[target.color][target.type].pos);
	SetCursorPosition(coordinate[0] - 4, coordinate[1]);
	ChangeColorMap(target.color, figure[target.color][target.type].pos);
	ChangeColor(figure[target.color][target.type].pos);
	for (int a = 0; a < 8; a++)
		std::cout << figure[target.color][target.type].name[a];
}

// FIXME: when you select figure then you exit and save other color is on turn
Gamestate Board::SelectFigure(bool &reSelect, Target* id)
{
	char input[3];
	int pos;

	//scanf_s("%s", &input);
	// TODO: add better protection after input maybe flushall will do it
	std::cin >> input;
	pos = ConvertUserCoordinatesToPos(input[0], input[1]);
	if ((input[0] < 'a' || input[0] > 'h' || input[1] < '1' || input[1] > '8' || TestForFigure(pos, true, false) == false)
		&& input[0] != 'x')
		bad = true;
	while (bad == true)
	{
		std::cout << "Nevazeci ulaz.\n";
		std::cin >> input;
		pos = ConvertUserCoordinatesToPos(input[0], input[1]);
		if ((input[0] < 'a' || input[0] > 'h' || input[1] < '1' || input[1] > '8' || TestForFigure(pos, true, false) == false)
			&& input[0] != 'x')
			bad = true;
		else bad = false;
	}
	if (input[0] == 'x')
		return Gamestate::EXIT;

	for (int j = 0; j < figure.size(); j++)
	{
		for (int i = 0; i < figure[j].size(); i++)
		{
			if (figure[j][i].pos == pos)
			{
				id->color = j;
				id->type = i;
				if (figure[j][i].name[5] == 'l' && figure[j][i].moves == 0)
					if (TestForCastling(j, i))// TODO: add target parameter
						return Gamestate::SELECT;

				if (TestForCheck(pos, (*id)) == true)
				{
					std::cout << "Ne mozete pomaknuti ovu figuru, doci ce do saha!\nOdaberi drugu figuru.\n";
					reSelect = true;
					return Gamestate::SELECT;
				}
				TestForMove(figure[j][i].pos, 0, showAvalibleSpots, false, 0, (*id));
				return Gamestate::MOVE;
			}
		}
	}
	//return Gamestate::SELECT;
}
// TODO: if figure does not have avalible paths print that it does'n have and select other figure
Gamestate Board::MoveFigure(bool &reSelect, Target* id)
{
	Target id0;
	char input[3];
	int pos;

	std::cin >> input;
	pos = ConvertUserCoordinatesToPos(input[0], input[1]);
	if ((input[0] < 'a' || input[0] > 'h' || input[1] < '1' || input[1] > '8' || TestForFigure(pos, true, false) == true) &&
		(input[0] != 'r' && input[1] != '\0'))
		bad = true;
	while (bad == true)
	{
		std::cout << "Nevazeci ulaz.\n";
		std::cin >> input;
		pos = ConvertUserCoordinatesToPos(input[0], input[1]);
		if ((input[0] < 'a' || input[0] > 'h' || input[1] < '1' || input[1] > '8' || TestForFigure(pos, true, false) == true) &&
			(input[0] != 'r' && input[1] != '\0'))
			bad = true;
		else bad = false;
	}
	if (input[0] == 'r')
	{
		RemovePossiblePaths();
		ClearChat();
		std::cout << "Odaberi drugu figuru.\n";
		reSelect = true;
		return Gamestate::SELECT;
	}
	else if (input[0] == 'x')
		return Gamestate::EXIT;

	RemovePossiblePaths();

	PrintBeforeMove((*id));

	//if we are eating figure we replace eated figure by last figure and we remove last figure
	if (SearchFigure(pos, &id0))
	{
		switch (figure[id0.color][id0.type].name[5])
		{
		case'p': eatenFigures[id0.color].top++;
			break;
		case'j': eatenFigures[id0.color].konj++;
			break;
		case'a': eatenFigures[id0.color].lovac++;
			break;
		case'i': eatenFigures[id0.color].kraljica++;
			break;
		case'u': eatenFigures[id0.color].pijun++;
			break;
		}
		PrintEatenFigures();
		figure[id0.color][id0.type] = figure[id0.color].back();
		figure[id0.color].pop_back();
	}

	//Changing figure stats
	figure[id->color][id->type].pos = pos;
	figure[id->color][id->type].moves++;

	//Pawn swapping
	if (figure[id->color][id->type].name[5] == 'u' && ((id->color == 0 && figure[id->color][id->type].pos > 56) ||
		(id->color == 1 && figure[id->color][id->type].pos < 9)))
		Promotion(id->color, id->type);

	PrintAfterMove((*id));

	ClearChat();
	return Gamestate::SELECT;
}