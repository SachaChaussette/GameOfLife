#pragma once

#include "Grid.h"

class Game
{
	HWND hwnd;
	HANDLE consoleHandle;
	CONSOLE_CURSOR_INFO cursorInfo;
	Grid* grid;

public:
	Game();

public:
	void Gameloop();
	void DisplayGrid();
};

