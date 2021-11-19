#include "Board.h"

//**********************************************************************************************************//
//*********************************************Convert functions********************************************//
//**********************************************************************************************************//

int Board::ConvertUserCoordinatesToPos(char inputX, char inputY)
{
	int pos = 1;

	for (char y = '1'; y <= '8'; y++)
	{
		for (char x = 'a'; x <= 'h'; x++, pos++)
		{
			if (inputX == x && inputY == y)
				return pos;
		}
	}
}

void Board::ConvertPosToCoordinate(int pos)
{
	int temp = pos;

	for (int a = 64; a >= 0; a -= 8)
	{
		if (temp > a) {
			temp -= a;
			coordinate[0] = temp * 9 - 2;
			break;
		}
		else if (temp == a)
		{
			coordinate[0] = 8 * 9 - 2;
			break;
		}
	}
	if (pos % 8 == 0)
		coordinate[1] = (9 - pos / 8) * 9 - 2;
	else
		coordinate[1] = (8 - pos / 8) * 9 - 2;
}

int Board::ConvertCoordinatesToPos(int x, int y)
{
	int pos = 0;

	for (int posy = 0; posy <= 63; posy += 9)
	{
		for (int posx = 0; posx <= 63; posx += 9)
		{
			pos++;
			if (x > 2 + posx && x < 12 + posx && y > 65 - posy && y < 75 - posy)
				return pos;
		}
	}
}

//YOU ARE NOT USING THIS FUNCTION
//void Board::ConvertPosToUserCoordinates(int pos)
//{
//	userCoordinates[0] = 'a';
//	userCoordinates[1] = '1';
//
//	for (int i = 1; i < 65; i++, userCoordinates[0]++)
//	{
//		if ((i - 1) % 8 == 0 && i != 1)
//		{
//			userCoordinates[0] = 'a';
//			userCoordinates[1]++;
//		}
//		if (i == pos)
//			return;
//	}
//}

//************************************************************************************************************//
//*********************************************Aesthetic functions********************************************//
//************************************************************************************************************//

void Board::SetCursorPosition(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Board::ChangeColor(int pos)
{
	if (pos == 0)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	else
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorMap[pos]);
}

void Board::ChangeColorMap(int type, int pos)
{
	if (type == 0)
		if (colorMap[pos] >= 240)
			colorMap[pos] = 252;
		else
			colorMap[pos] = 12;
	else
		if (colorMap[pos] >= 240)
			colorMap[pos] = 249;
		else
			colorMap[pos] = 9;
}

void Board::ClearChat()
{
	SetCursorPosition(p.x, p.y);
	ChangeColor(0);

	for (int a = 0; a < 17; a++)
	{
		for (int b = 0; b < 32; b++)
			std::cout << ' ';
		std::cout << std::endl;
	}
	SetCursorPosition(p.x, p.y);
}

//********************************************************************************************************//
//*********************************************Board functions********************************************//
//********************************************************************************************************//

void Board::SetBoard()
{
	Figure figure1;
	figure.clear();
	//Postavljam koordinate
	for (int x = 7, i = 0; x < 79; x += 9, i++)
	{
		board[x][0] = 97 + i;
		board[x][78] = 97 + i;
		board[0][x] = 56 - i;
		board[78][x] = 56 - i;
	}
	//Postavljam rub
	for (int y = 0; y < 79; y++)
	{
		for (int x = 0; x < 79; x++)
		{
			if ((y == 2 || y == 75) && (x > 2 && x < 75))
				board[x][y] = '-';
			else if ((x == 2 || x == 75) && (y > 2 && y < 75))
				board[x][y] = '|';
			else if (board[x][y] == -52)
				board[x][y] = ' ';
		}
	}
	//Postavljam figure
	for (int j = 0, n = 1; j < 2; j++)
	{
		std::vector<Figure> temp;
		for (int i = 0; i < 16; i++, n++)
		{
			if (n == 65)
				n = 49;

			if (i > 7)
				figure1.name = "  Pijun  ";
			else if (i == 0 || i == 7) 
				figure1.name = "   Top   ";
			else if (i == 1 || i == 6)
				figure1.name = "  Konj   ";
			else if (i == 2 || i == 5)
				figure1.name = "  Lovac  ";
			else if (i == 3)
				figure1.name = "Kraljica ";
			else if (i == 4)
				figure1.name = "  Kralj  ";

			figure1.pos = n;
			//king is first on the list
			if (i == 4)
				temp.insert(temp.begin(), figure1);
			else
				temp.push_back(figure1);
		}
		n = 57;
		figure.push_back(temp);
	}

	p.x = 0;
	p.y = 80;
	//Postavljam boje
	for (int i = 1; i < 65; i++)
	{
		static bool New = false;

		if (New == false)
			if (i % 2 == 0)
				colorMap[i] = 240;
			else
				colorMap[i] = 15;
		else
			if (i % 2 == 0)
				colorMap[i] = 15;
			else
				colorMap[i] = 240;

		if (i % 8 == 0)
			if (New == false)
				New = true;
			else
				New = false;
	}
}

void Board::PrintEatenFigures()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	SetCursorPosition(80, 3);
	std::cout << "Pijun x " << eatenFigures[0].pijun;
	SetCursorPosition(80, 5);
	std::cout << "Top x " << eatenFigures[0].top;
	SetCursorPosition(80, 7);
	std::cout << "Konj x " << eatenFigures[0].konj;
	SetCursorPosition(80, 9);
	std::cout << "Lovac x " << eatenFigures[0].lovac;
	SetCursorPosition(80, 11);
	std::cout << "Kraljica x " << eatenFigures[0].kraljica;
	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
	SetCursorPosition(80, 66);
	std::cout << "Pijun x " << eatenFigures[1].pijun;
	SetCursorPosition(80, 68);
	std::cout << "Top x " << eatenFigures[1].top;
	SetCursorPosition(80, 70);
	std::cout << "Konj x " << eatenFigures[1].konj;
	SetCursorPosition(80, 72);
	std::cout << "Lovac x " << eatenFigures[1].lovac;
	SetCursorPosition(80, 74);
	std::cout << "Kraljica x " << eatenFigures[1].kraljica;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	SetCursorPosition(0, 80);
}

void Board::PrintBoard()
{
	Target id0;
	int pos;
	bool haveFigureName = false;

	for (int y = 0; y < 79; y++)
	{
		for (int x = 0; x < 79; x++)
		{
			if (x > 2 && x < 75 && y > 2 && y < 75)
			{
				if ((x - 2) % 9 == 0 || (y - 2) % 9 == 0 || x == 3 || y == 3 || x != 74 || y != 74)
				{
					pos = ConvertCoordinatesToPos(x, y);
					for (int j = 0; j < figure.size(); j++)
					{
						for (int i = 0; i < figure[j].size(); i++)
						{
							if (figure[j][i].pos == pos)
							{
								if (y == 7 || y == 16 || y == 25 || y == 34 || y == 43 || y == 52 || y == 61 || y == 70)
									haveFigureName = true;
								id0.color = j;
								id0.type = i;
								ChangeColorMap(j, pos);
								j = 1;
								break;
							}
						}
					}
					ChangeColor(pos);
				}
			}
			else ChangeColor(0);
			if (haveFigureName == true && x > 2)
			{
				std::cout << figure[id0.color][id0.type].name;
				x += 8;
				haveFigureName = false;
			}
			else
				std::cout << board[x][y];
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	PrintEatenFigures();
}

bool Board::IsGamePlayed()
{
	for (int j = 0; j < figure.size(); j++)
	{
		for (int i = 0; i < figure[j].size(); i++)
		{
			if (figure[j][i].moves != 0)
				return true;
		}
	}
	return false;
}