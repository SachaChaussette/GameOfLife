#pragma once

#include "Tile.h"

class Grid
{
	vector<vector<Tile>> tiles;
	u_int length;
	u_int width;

public:
	Grid() = default;
	Grid(const u_int& _length, const u_int& _width);
private:
	void Setup();
public:
	void Display(const bool _withGrid) const;
};

