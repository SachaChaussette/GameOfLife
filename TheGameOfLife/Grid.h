#pragma once

#include "Tile.h"

class Grid
{
	vector<vector<Tile*>> tiles;
	u_int length;
	u_int width;

public:
	u_int GetLength()
	{
		return length;
	}
	u_int GetWidth()
	{
		return width;
	}
public:
	Grid();
	Grid(const u_int& _length, const u_int& _width);
private:
	void Setup();
public:
	void Display(const bool _withGrid, const pair<int, int>& _indexesToSelect) const;
};

