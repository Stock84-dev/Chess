#include <iostream>
#include <windows.h>
#include <fstream>
using namespace std;

void Color()
{
	int b;
	ShellExecute(NULL, "open", "https://www.chess.com/learn-how-to-play-chess", NULL, NULL, SW_SHOWNORMAL);
	//system("<InternetXplorer> http://google.com");

	for (int colour = 0x00; colour <= 0xff; colour++)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colour);
		cout << "Using colour:" << colour << endl;
		//cin.get();
	}
	cin >> b;
}

void Cursor()
{//Handle hConsoleOutput, COORD
	using namespace std;
	COORD coord;
	int line, column;
	int b = 1;

	for (int j = 0; j < 10; j++)
	{
		for (int i = 0; i < 10; i++)
		{
			cout << "Cursor!!!";
		}
	}
	while (b < 100)
	{
		b++;
		cin >> line;
		cin >> column;
		coord.X = line;
		coord.Y = column;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	}
	
		
	
	
	
}

int main()
{
	int a;
	//cin.get();
	cin >> a;
	if (a == 1)
		Color();
	else Cursor();

	
	return 0;
}
