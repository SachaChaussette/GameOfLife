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

void Grid::Display(const bool _withGrid, const pair<int, int>& _indexesToSelect, const bool _isDebug) const
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
			string _firstSymbol = " ";
			string _secondSymbol = " ";

			if (_withGrid) DISPLAY("|", false);

			if (_indexesToSelect.first == _colIndex && _indexesToSelect.second == _rowIndex && _withGrid)
			{
				_firstSymbol = "[";
				_secondSymbol = "]";
				DISPLAY(_firstSymbol, false);
				tiles[_rowIndex][_colIndex]->Display(_isDebug);
				DISPLAY(_secondSymbol, false);
			}
			else if (_indexesToSelect.first == _colIndex && _indexesToSelect.second == _rowIndex)
			{		
				DISPLAY(BG_DARK_GRAY, false);
				DISPLAY("  ", false);
				DISPLAY(RESET, false);
			}
			else if(_withGrid)
			{
				DISPLAY(" ", false);
				tiles[_rowIndex][_colIndex]->Display(_isDebug);
				DISPLAY(" ", false);
			}
			else
			{
				DISPLAY("", false);
				tiles[_rowIndex][_colIndex]->Display(_isDebug);
				DISPLAY("", false);
			}

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

string Grid::ToString(const bool _withGrid) const
{
	string _text;

	for (u_int _rowIndex = 0; _rowIndex < length; _rowIndex++)
	{
		if (_withGrid)
		{
			_text += "+";
			for (u_int _index = 0; _index < width; _index++)
			{
				_text += "----";
				_text += "+";
			}
			_text += "\n";
		}

		for (u_int _colIndex = 0; _colIndex < width; _colIndex++)
		{
			if (_withGrid) _text += "| ";
			_text += tiles[_rowIndex][_colIndex]->ToString(false);
			_text += " ";
		}
		_text += _withGrid ? "|\n" : "\n";
	}
	if (_withGrid)
	{
		for (u_int _index = 0; _index < width; _index++)
		{
			_text += "+";
			_text += "----";
		}
		_text += "+\n";
	}

	return _text;
}
