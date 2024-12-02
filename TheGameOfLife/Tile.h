#pragma once

#include "Coordinate.h"

class Tile
{
	Coordinate coordinate;
	string appearance;
	u_int weight;
public:
	void SetTile(const Coordinate& _coordinate)
	{
		coordinate = _coordinate;
	}
public:
	Tile(const Coordinate& _coordinate);
	
	Tile(const string& _appearance, const u_int& _weight);
	
public:
	void UpdateCell(const string& _appearance, const u_int& _weight);
	
	void Display() const;
	
};

