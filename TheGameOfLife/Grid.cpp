#include "Grid.h"

Grid::Grid()
{
	length = 10;
	width = 10;
	Setup();
}

Grid::Grid(const u_int& _length, const u_int& _width)
{
	length = _length;
	width = _width;
	Setup();
}

Grid::~Grid()
{
	for (u_int _rowIndex = 0; _rowIndex < length; _rowIndex++)
	{
		for (u_int _colIndex = 0; _colIndex < width; _colIndex++)
		{
			delete tiles[_rowIndex][_colIndex];
		}

	}
}

void Grid::Setup()
{
	// vector temporaire
	for (u_int _rowIndex = 0; _rowIndex < length; _rowIndex++)
	{
		vector<Tile*> _temp;
		for (u_int _colIndex = 0; _colIndex < width; _colIndex++)
		{
			Tile* _newTile = new Tile(new Coordinate(_rowIndex, _colIndex));
			_temp.push_back(_newTile);
		}

		// ajoute la ranger de Tile à la Grid
		tiles.push_back(_temp);

	}
}

void Grid::Display(const bool _withGrid, const pair<int, int>& _indexesToSelect)
{
	for (u_int _rowIndex = 0; _rowIndex < length; _rowIndex++)
	{
		if (_withGrid)
		{
			DISPLAY("+", false);
			for (u_int _index = 0; _index < width; _index++)
			{
				DISPLAY("----", false);
				DISPLAY("+", false);
			}
			DISPLAY("", true);
		}
		
		for (u_int _colIndex = 0; _colIndex < width; _colIndex++)
		{
			if (_withGrid) 
			{
				DISPLAY("|  ", false);
			}
			DISPLAY("  ", false);
		}
		if (_withGrid) DISPLAY("|", true);
	}
	if (_withGrid)
	{
		for (u_int _index = 0; _index < width; _index++)
		{
			DISPLAY("+", false);
			DISPLAY("----", false);
		}
		DISPLAY("+", true);
	}
}
