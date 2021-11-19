#include "Interface.h"

//POTREBNA DIMENZIJA PROZORA 8X8
//Look up on how to use intellisense(function comment)
int main()
{
	Interface menu;
	menu.board1.SetBoard();
	menu.board1.playerTurn = true;
	menu.Menu();
	return 0;
}