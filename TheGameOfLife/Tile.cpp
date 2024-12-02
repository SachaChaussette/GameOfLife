#include "Tile.h"

Tile::Tile(const Coordinate& _coordinate)
{
	coordinate = _coordinate;
	appearance = ".";
	weight = 0;
}

Tile::Tile(const string& _appearance, const u_int& _weight)
{
	appearance = _appearance;
	weight = _weight;
}

void Tile::UpdateCell(const string& _appearance, const u_int& _weight)
{
	weight = _weight;
	appearance = _appearance;
	// TODO UPDATE LIST IN GAME
}

void Tile::Display() const
{
	DISPLAY(appearance, false);
}
