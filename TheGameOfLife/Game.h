#pragma once

#include "Grid.h"

enum InputType
{
	IT_ENTER = 13,
	IT_ECHAP = 27,
	IT_UP = 122,
	IT_DOWN = 115,
	IT_LEFT = 113,
	IT_RIGHT = 100,
	IT_A = 97,
	IT_E = 101,
	IT_F = 102,
	IT_G = 103,
	IT_DEBUG = 253,

	IT_COUNT,
};

enum ActionType
{
	AT_GRID = 0,
	AT_QUIT,
};

class Game
{
	InputType inputType;
	HWND hwnd;
	HANDLE consoleHandle;
	CONSOLE_CURSOR_INFO cursorInfo;
	Grid* grid;
	vector<Coordinate*> coordinatesCellAlive;
	u_int iterationCount;
public:
	Game();
	Game(const int _width, const int _length);
	~Game();
private:
	void NextIteration();
	void PushCellAlive(Coordinate* _coordinate);
	void PushCellAlive(const Coordinate& _coordinate);
	void CheckCellAlive();
	void ResetWeightCell();

	pair<int, int> ChooseInputAndRetrieveCoords(const u_int& _optionsCount, pair<int, int> _pairOfIndexes);
	int ChooseInputAndRetrieveIndex(const u_int& _optionsCount, int _currentIndex);
	void DisplayMenu(const string* _options, const int& _indexToSelect, const u_int& _optionsCount, const string& _question);
	void SelectionMenu();
	void GridMenu();
	bool SelectionMenu(const int _menuIndex);
	bool IsAlreadyAlive(const pair<int, int>& _pairOfIndexes);
	


public:
	void DisplayGrid(const bool _showGrid = false);
	void DisplayInfos();
	void AddPointToNeighbourCell();
	bool IsOver();
	void Loop();
};

