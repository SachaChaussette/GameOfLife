#include "Tile.h"

Tile::Tile(Coordinate* _coordinate)
{
	coordinate = _coordinate;
	appearance = RESET;
	weight = 0;
}

Tile::Tile(const string& _appearance, const u_int& _weight)
{
	appearance = _appearance;
	weight = _weight;
}

void Tile::UpdateWeight(const int _point)
{
	weight += _point;
	weight %= 4;
	if (weight != 3 && appearance == RESET) return;
	else if (weight == 3 && appearance == RESET)
	{
		appearance = WHITE_BG;
		return;
	}
	else
	{
		appearance = RESET;
	}
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
