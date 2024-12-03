#pragma once

#include "Grid.h"

class Game
{
	Grid grid;
	vector<Tile> tilesWicthCell;
	u_int iterationCount;
	bool isPause;
public:
	Game(const Grid& _grid);
private:
	void NextIteration();
	void GiveWeightToTile();
	bool IsOver();
	void SelectionMenu();
	void GridMenu();
	void ChooseMenu(const int _menuIndex);
	int ChooseInputAndRetrieveIndex(const u_int& _optionsCount, int _currentIndex);
public:
	void Loop();
};

