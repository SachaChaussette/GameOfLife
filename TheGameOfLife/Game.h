#pragma once

#include "Grid.h"

class Game
{
	HWND hwnd;
	HANDLE consoleHandle;
	CONSOLE_CURSOR_INFO cursorInfo;
	Grid* grid;
	vector<Coordinate*> coordinatesCellAlive;

public:
	Game();
	Game(const int _width, const int _length);
	~Game();

private:
	void PushCellAlive(Coordinate* _coordinate);
	void PushCellAlive(const Coordinate& _coordinate);
	void CheckCellAlive();
	void ResetWeightCell();
	void InitGlider();

public:
	void Gameloop();
	bool IsOver();
	void DisplayGrid(const bool _showGrid = false);
	void AddPointToAll();
	void AddPointToNeighbourCell();
};

