#pragma once

#include "Tile.h"

class Grid
{
	vector<vector<Tile*>> tiles;
	u_int length;
	u_int width;

public:
	Grid();
	Grid(const u_int& _length, const u_int& _width);
private:
	void Setup();
public:
	void Display(const bool _withGrid = false) const;
	string ToString(const bool _withGrid = false) const;
};

