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
public:
	void Loop();
};

