#pragma once

#include "..\GameOfLifeLib\Macro.h"

struct Coordinate
{
	int x;
	int y;

	Coordinate() = default;
	Coordinate(const int _x, const int _y)
	{
		x = _x;
		y = _y;
	}
};