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

	bool operator==(Coordinate _other)
	{
		return _other.y == y && _other.x == x;
	}

	bool operator==(Coordinate* _other)
	{
		return _other->y == y && _other->x == x;
	}

};