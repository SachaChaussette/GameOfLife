#pragma once

#include "Tile.h"

class Grid
{
	vector<vector<Tile*>> tiles;
	u_int length;
	u_int width;

public:
	inline Tile* GetTile(const int _x, const int _y) const
	{
		return tiles[_x][_y];
	}

	inline Tile* GetTile(Coordinate* _coordinate) const
	{
		return tiles[_coordinate->x][_coordinate->y];
	}

	inline Tile* GetTile(Coordinate _coordinate) const
	{
		return tiles[_coordinate.x][_coordinate.y];
	}

	inline u_int GetLength() const
	{
		return length;
	}

	inline u_int GetWidth() const
	{
		return width;
	}
public:
	Grid();
	Grid(const u_int& _length, const u_int& _width);
	~Grid();
private:
	void Setup();
public:
	void Display(const bool _withGrid, const pair<int, int>& _indexesToSelect, const bool _isDebug) const;
	string ToString(const bool _withGrid = false) const;
};

