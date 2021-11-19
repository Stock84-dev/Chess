#pragma once
#include "Figure.h"
#include <windows.h>
#include <vector>
#include <string>
#include <iostream>
// TODO: add dimmer green color when possible paths are printed on dark piece
// TODO: add singleplayer with simple ai

enum class Gamestate { SELECT, MOVE, EXIT };

class Board
{
public:
	struct EatenFigures
	{
		int pijun = 0;
		int top = 0;
		int konj = 0;
		int lovac = 0;
		int kraljica = 0;
	};
	//Board.cpp
	int coordinate[2];
	//int targetFigure[2];
	char userCoordinates[2];
	POINT p; //point with coordinates
	int colorMap[65];
	char board[79][79];
	EatenFigures eatenFigures[2];
	
	struct Target
	{
		int color;
		int type;
	};

	std::vector<int> possiblePaths;
	std::vector<std::vector<Figure>> figure;

	//Figure.cpp
	bool bad = false;
	int selectedFigure[2];

	//Out
	bool playerTurn;
	bool showAvalibleSpots = true;
	
	//*********************************************Searching for figure functions********************************************//

	//Searches figure if there is figure on that pos also returns figure type in targetFigure[]
	bool SearchFigure(int pos);
	bool SearchFigure(int pos, Target* ID);
	//Tests if there is figure on that position with better percision(pos, ownFigure, don't care which figure)
	bool TestForFigure(int pos, bool ownFigure, bool ignore = false);

	//*********************************************Testing for move functions********************************************//

	//Prints paths where selected figure can go
	void PrintPossiblePaths(int i, int colorId);
	//Removes paths where selected figure can go
	void RemovePossiblePaths();
	void PawnMove(int targetPos, int forceSearch, bool show, Target id);
	void TowerMove(int targetPos, int forceSearch, bool show, int ignorePos, Target id);
	void HorseMove(int targetPos, int forceSearch, bool show, Target id);
	void HunterMove(int targetPos, int forceSearch, bool show, int ignorePos, Target id);
	void KingMove(int targetPos, int forceSearch, bool show, Target id);
	//Tests a selected figure (or on target pos with force search) where it can move and prints it or not 
	//(figure[targetFigure[0]][targetFigure[1]].name[5] == 'p', 1 top
	//(j, 2) konj, (a, 3) lovac, (i, 4) kraljica, (l, 5) kralj, (u, 6) pijun 
	void TestForMove(int targetPos, int forceSearch, bool show, bool stackPaths, int ignorePos, Target id);
	bool TestForCheck(int pos, Target id);

	bool TestForCheckAtPos(int pos, bool AllyColor, int ignorePos);

	//*********************************************Manipulating figure functions********************************************//
	
	//returns true if player has done it
	bool TestForCastling(int color, int type);
	void Promotion(int color, int type);
	void PrintBeforeMove(Target target);
	void PrintAfterMove(Target target);
	Gamestate SelectFigure(bool &reSelect, Target* id);
	Gamestate MoveFigure(bool &reSelect, Target* id);

	//*********************************************Convert functions********************************************//

	//Returns position from character coordinates(a-h)(1-8)
	int ConvertUserCoordinatesToPos(char x, char y);
	//Returns coordinates to coordinate[]
	void ConvertPosToCoordinate(int pos);//should return class COORD OR POINT
										 //Returns position from "board" coordinates
	int ConvertCoordinatesToPos(int x, int y);
	//void ConvertPosToUserCoordinates(int pos);//YOU ARE NOT USING THIS FUNCTION

	//*********************************************Aesthetic functions********************************************//

	//Sets cursor position to "board" coordinates
	void SetCursorPosition(int x, int y);
	//Changes color from colorMap, if pos = 0 changes to default color
	void ChangeColor(int pos);
	//Changes colormap using figure type and position
	void ChangeColorMap(int type, int pos);
	//Clears chat where user interacts with game
	void ClearChat();

	//*********************************************Board functions********************************************//
	
	//Sets chessboard
	void SetBoard();
	void PrintEatenFigures();
	//Prints chessboard
	void PrintBoard();
	bool IsGamePlayed();

	//*********************************************File streams***********************************************//

	bool SaveGame(std::string input);
	bool LoadGame(std::string input);
	bool SaveGameData(std::vector<std::string> &loadNames);
	bool LoadGameData(std::vector<std::string> &loadNames);
};