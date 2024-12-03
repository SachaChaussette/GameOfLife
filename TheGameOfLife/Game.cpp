#include "Game.h"

Game::Game()
{
	hwnd = GetForegroundWindow();
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	cursorInfo.dwSize = 100;
	cursorInfo.bVisible = FALSE;
	grid = new Grid();
}

void Game::Gameloop()
{
	do
	{
		SetConsoleCursorInfo(consoleHandle, &cursorInfo);
		DisplayGrid();

	} while (true);
}

void Game::DisplayGrid()
{
	string _gridString = grid->ToString();
	DISPLAY(_gridString, true);
}
