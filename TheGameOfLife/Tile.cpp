#include "Tile.h"

Tile::Tile(Coordinate* _coordinate)
{
	state = CT_DEAD;
	coordinate = _coordinate;
	appearance = RESET;
	neighbourCount = 0;
}



Tile::~Tile()
{
	delete coordinate;
}

void Tile::ResetNeigbourCount()
{
	if (!(neighbourCount == 3)) neighbourCount = 0;
}

/// <summary>
/// Permet de changer le poids de la cell,
/// Si le poid est de 3 alors la méthode renvoie
/// sa coordonée
/// Sinon elle renvoie nullptr
/// </summary>
/// <param name="_point"></param>
/// <returns></returns>
void Tile::UpdateNeighbourCount(const int _point)
{
	neighbourCount += _point;
}

CellState Tile::UpdateCellState()
{
	if (neighbourCount == 2)
	{
		return state;
	}
	else if (neighbourCount == 3)
	{
		appearance = BG_WHITE;
		state = CT_ALIVE;
		return state;
	}
	else
	{
		appearance = RESET;
		state = CT_DEAD;
		return state;
	}
}

void Tile::Display(const bool _debug) const
{
	if (_debug)
	{
		DISPLAY(to_string(neighbourCount) + " " + RESET, false);
	}
	else
	{
		DISPLAY(appearance + "  " + RESET, false)
	}
}

string Tile::ToString(const bool _debug)
{
	if (_debug) return " " + to_string(neighbourCount);
	return appearance + "  " + RESET;
}

void Tile::RemoveCell()
{
	neighbourCount = 0;
	UpdateCellState();
}