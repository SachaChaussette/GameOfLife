#include "Grid.h"

Grid::Grid(const u_int& _length, const u_int& _width)
{
	length = _length;
	width = _width;
	Setup();
}

void Grid::Setup()
{
	// vector temporaire
	vector<Tile> _temp;
	for (int _rowIndex = 0; _rowIndex < length; _rowIndex++)
	{
		for (int _colIndex = 0; _colIndex < width; _colIndex++)
		{
			_temp.push_back(Coordinate(_rowIndex , _colIndex ));
		}

		// ajoute la ranger de Tile à la Grid
		tiles.push_back(_temp);

		// pop sinon _temp va garder les précédents en mémoire
		for (int _index = 0; _index < width; _index++)
		{
			_temp.pop_back();
		}
	}
}

void Grid::Display(const bool _withGrid) const
{
	
	for (u_int _rowIndex = 0; _rowIndex < length; _rowIndex++)
	{
		if (_withGrid)
		{
			DISPLAY("+", false);
			for (u_int _index = 0; _index < width; _index++)
			{
				DISPLAY("---", false);
				DISPLAY("+", false);
			}
			DISPLAY("", true);
		}
		
		for (u_int _colIndex = 0; _colIndex < width; _colIndex++)
		{
			if (_withGrid) DISPLAY("| ", false);
			tiles[_rowIndex][_colIndex].Display();
			DISPLAY(" ", false);
		}
		if (_withGrid) DISPLAY("|", true);
	}
	if (_withGrid)
	{
		for (u_int _index = 0; _index < width; _index++)
		{
			DISPLAY("+", false);
			DISPLAY("---", false);
		}
		DISPLAY("+", true);
	}
}
