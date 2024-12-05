#pragma once

#include "Coordinate.h"


enum CellState
{
	CT_DEAD,
	CT_ALIVE,
};


class Tile
{
	Coordinate* coordinate;
	CellState state;
	string appearance;
	u_int neighbourCount;
public:
	void SetTile(Coordinate* _coordinate)
	{
		coordinate = _coordinate;
	}
	inline void SetAppearance(const string& _color)
	{
		appearance = _color;
	}


	inline void SetWeight(const u_int _value)
	{
		neighbourCount = _value;
	}

	inline void SetState(const CellState& _cellState)
	{
		state = _cellState;
	}
	inline Coordinate* GetCoordinate()const
	{
		return coordinate;
	}

	inline u_int GetNeighbourCount()const
	{
		return neighbourCount;
	}

	inline CellState GetCellState()const
	{
		return state;
	}
public:
	Tile(Coordinate* _coordinate);

	Tile(const string& _appearance, const u_int& _weight);

	~Tile();

public:
	void UpdateNeighbourCount(const int _point);
	CellState UpdateCellState();
	void ResetNeigbourCount();
	void Display(const bool _debug) const;
	void RemoveCell();
	string ToString(const bool _debug);

};

