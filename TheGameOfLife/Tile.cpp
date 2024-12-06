#include "Tile.h"

Tile::Tile(Coordinate* _coordinate)
{
	state = CT_DEAD;
	coordinate = _coordinate;
	appearance = RESET;
	neighbourCount = 0;
}

Tile::Tile(const string& _appearance, const u_int& _weight)
{
	appearance = _appearance;
	neighbourCount = _weight;
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

CellState Tile::UpdateCellState(/*const int _appearanceType*/)
{
	if (neighbourCount == 2) return state;
	else if (neighbourCount == 3)
	{
		/*switch (_appearanceType)
		{
		case 0:
			appearance = BG_WHITE;
			break;
		case 1:
			appearance = BG_DARK_GRAY;
			break;
		case 2:
			appearance = BG_GREEN;
			break;
		case 3:
			appearance = BG_RED;
			break;
		case 4:
			appearance = RESET;
			break;
		default:
			break;
		}*/
		
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