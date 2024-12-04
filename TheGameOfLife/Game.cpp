#include "Game.h"

Game::Game()
{
	hwnd = GetForegroundWindow();
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	cursorInfo.dwSize = 100;
	cursorInfo.bVisible = FALSE;
	grid = new Grid();
}

Game::Game(const int _width, const int _length)
{
	hwnd = GetForegroundWindow();
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	cursorInfo.dwSize = 100;
	cursorInfo.bVisible = FALSE;
	grid = new Grid(_width, _length);
}

Game::~Game()
{
	delete grid;
}


void Game::AddPointToNeighbourCell()
{
	
	ResetWeightCell();
	Coordinate _directions[]
	{
		Coordinate(0,1),
		Coordinate(1,1),
		Coordinate(1,0),
		Coordinate(1,-1),
		Coordinate(0,-1),
		Coordinate(-1,-1),
		Coordinate(-1,0),
		Coordinate(-1,1),
	};

	vector<Coordinate> _garbageCoord;
	const u_int _coordinatesCellAliveSize = (u_int)coordinatesCellAlive.size();
	for (u_int _index = 0; _index < _coordinatesCellAliveSize; _index++)
	{
		Coordinate* _currentCoordinate = coordinatesCellAlive[_index];
		for (Coordinate _direction : _directions)
		{
			Coordinate _coordinateToChange = ComputeNewCoordinate(_currentCoordinate, _direction);
			if (_coordinateToChange.x < 0 || _coordinateToChange.y < 0) continue;
			Tile* _tileToUpdate = grid->GetTile(_coordinateToChange);
			_tileToUpdate->UpdateNeighbourCount(1);
			PushInVectorUnique(_garbageCoord, _coordinateToChange);
		}
		
	}
	const u_int& _garbageSize = (u_int)_garbageCoord.size();
	for (u_int _index = 0; _index< _garbageSize; _index++)
	{
		PushCoordinateCellAlive(_garbageCoord[_index]);
		if (NotInAliveCell(_garbageCoord[_index]))
		{
			grid->GetTile(_garbageCoord[_index])->ResetNeigbourCount();
		}
	}

}

void Game::CheckCellAlive()
{
	u_int _coordinatesCellAliveSize = (u_int)coordinatesCellAlive.size();
	for (u_int _index = 0; _index < _coordinatesCellAliveSize; _index++)
	{
		Coordinate* _coordinateToCheck = coordinatesCellAlive[_index];
		Tile* _tileToUpdate = grid->GetTile(_coordinateToCheck);
		if (_tileToUpdate->GetNeighbourCount() == 0)
		{
			_tileToUpdate->UpdateCellState();
		}
		if (_tileToUpdate->GetCellState() == CT_DEAD)
		{
 			_tileToUpdate->SetAppearance(RESET);
			coordinatesCellAlive.erase(coordinatesCellAlive.begin()+_index);
			_coordinatesCellAliveSize = (u_int)coordinatesCellAlive.size();
			--_index;
		}
	}
}

Coordinate Game::ComputeNewCoordinate(Coordinate* _currentCoordinate, const Coordinate& _direction)
{
	int _newX;
	if ((_newX = _currentCoordinate->x + _direction.x) < 0 || (u_int)_newX >= grid->GetLength())
	{
		_newX = -1;
	}

	int _newY;
	if ((_newY = _currentCoordinate->y + _direction.y) < 0 || (u_int)_newY >= grid->GetWidth())
	{
		_newY = -1;
	}

	return { _newX, _newY };
}

void Game::PushCoordinateCellAlive(Coordinate* _coordinate)
{
	Tile* _tileToUpdate = grid->GetTile(_coordinate);
	CellState _currentCellState = _tileToUpdate->UpdateCellState();
	if (_currentCellState == CT_ALIVE)
	{
		Coordinate* _coordinateToPush = _tileToUpdate->GetCoordinate();
		u_int _coordinatesCellAliveSize = (u_int)coordinatesCellAlive.size();
		for (u_int _index = 0; _index < _coordinatesCellAliveSize; _index++)
		{
			if (coordinatesCellAlive[_index] == _coordinateToPush) return;
		}
		coordinatesCellAlive.push_back(_coordinateToPush);
	}
}

void Game::PushCoordinateCellAlive(const Coordinate& _coordinate)
{
	Tile* _tileToUpdate = grid->GetTile(_coordinate);
	CellState _currentCellState = _tileToUpdate->UpdateCellState();
	if (_currentCellState == CT_ALIVE)
	{
		Coordinate* _coordinateToPush = _tileToUpdate->GetCoordinate();
		u_int _coordinatesCellAliveSize = (u_int)coordinatesCellAlive.size();
		for (u_int _index = 0; _index < _coordinatesCellAliveSize; _index++)
		{
			if (coordinatesCellAlive[_index] == _coordinateToPush) return;
		}
		coordinatesCellAlive.push_back(_coordinateToPush);
		sort(coordinatesCellAlive.begin(), coordinatesCellAlive.end());
	}
}

/// <summary>
///  Permet de remettre à 0 le neighbourCount des tiles dans le vector coordinatesCellAlive
/// </summary>
void Game::ResetWeightCell()
{
	const u_int _coordinatesCellAliveSize = (u_int)coordinatesCellAlive.size();
	for (u_int _index = 0; _index < _coordinatesCellAliveSize; _index++)
	{
		Coordinate* _currentCoordinate = coordinatesCellAlive[_index];
		Tile* _tempTile = grid->GetTile(_currentCoordinate);
		_tempTile->SetWeight(0);
	}
	oldCoordinatesCellAlive = coordinatesCellAlive;
}

/// <summary>
/// Fonction display qui permet de directement display aux coordonner les cellules vivant ou de les undisplay
/// </summary>
/// <param name="_cellCoordinates"></param>
/// <param name="_unDisplay"></param>
bool Game::NotInAliveCell(Coordinate _coordinateToCheck)
{
	const u_int _coordinatesCellAliveSize = (u_int)coordinatesCellAlive.size();
	for (u_int _index = 0; _index < _coordinatesCellAliveSize; _index++)
	{
		if (_coordinateToCheck == coordinatesCellAlive[_index]) return false;
	}
	return true;
}

void Game::DisplayCell(const vector<Coordinate*>& _cellCoordinates, const bool _unDisplay)
{
	u_int _coordinatesCellAliveSize = (u_int)_cellCoordinates.size();
	for (u_int _index = 0; _index < _coordinatesCellAliveSize; _index++)
	{
		Coordinate* _coordinate = _cellCoordinates[_index];
		SetConsoleCursorPosition(consoleHandle, { static_cast<short>(_coordinate->x * 2), static_cast<short>(_coordinate->y)});
		if (_unDisplay)
		{
			if (binary_search(coordinatesCellAlive.begin(), coordinatesCellAlive.end(), _cellCoordinates[_index])) continue;
			DISPLAY(string(RESET) + "  ", false);
		}
		else
		{
			grid->GetTile(_coordinate)->Display();
		}
	}
	if (!_unDisplay) Sleep(100);
}


/// <summary>
/// Boucle de jeux
/// </summary>
void Game::Gameloop()
{
	u_int _generation = 0; 
	SetConsoleCursorInfo(consoleHandle, &cursorInfo);
	//InitNewAliveCell(0 , 1);
	//InitGlider();
	//InitGun(10 , 10);
	InitPrimordialSoup();
	do
	{
		if (_generation % 50 == 1) system("cls");
		_generation += 1;
		SetConsoleCursorPosition(consoleHandle, { 0, 0});
		DISPLAY("Generation: " + to_string(_generation), false);
		DisplayCell(coordinatesCellAlive);
		AddPointToNeighbourCell();
		CheckCellAlive();
		DisplayCell(oldCoordinatesCellAlive, true);
		

	} while (!IsOver());
}

/// <summary>
/// Permet d'initialiser un glider sur une coordonnée donner
/// </summary>
/// <param name="_x"></param>
/// <param name="_y"></param>
void Game::InitGlider(const int _x, const int _y)
{
	InitNewAliveCell(0 + _x, 1 + _y);
	InitNewAliveCell(1 + _x, 2 + _y);
	InitNewAliveCell(2 + _x, 0 + _y);
	InitNewAliveCell(2 + _x, 1 + _y);
	InitNewAliveCell(2 + _x, 2 + _y);
}

void Game::InitBlock(const int _x, const int _y)
{
	InitNewAliveCell(0 + _x, 0 + _y);
	InitNewAliveCell(0 + _x, 1 + _y);
	InitNewAliveCell(1 + _x, 0 + _y);
	InitNewAliveCell(1 + _x, 1 + _y);
}

void Game::InitGunPart1(const int _x, const int _y)
{
	InitNewAliveCell(10 + _x, _y);
	InitNewAliveCell(10 + _x, 1 + _y);
	InitNewAliveCell(10 + _x, 2 + _y);

	InitNewAliveCell(11 + _x, -1 + _y);
	InitNewAliveCell(11 + _x, 3 + _y);

	InitNewAliveCell(12 + _x, -2 + _y);
	InitNewAliveCell(12 + _x, 4 + _y);

	InitNewAliveCell(13 + _x, -2 + _y);
	InitNewAliveCell(13 + _x, 4 + _y);

	InitNewAliveCell(14 + _x, 1 + _y);
}

void Game::InitGun(const int _x, const int _y)
{
	InitBlock(_x, _y);
	InitBlock(_x + 34, _y - 2);

	InitGunPart1(_x, _y);

	InitNewAliveCell(15 + _x, -1 + _y);
	InitNewAliveCell(15 + _x, 3 + _y);

	InitNewAliveCell(16 + _x, _y);
	InitNewAliveCell(16 + _x, 1 + _y);
	InitNewAliveCell(16 + _x, 2 + _y);

	InitNewAliveCell(17 + _x, 1 + _y);

	InitNewAliveCell(20 + _x, -2 + _y);
	InitNewAliveCell(20 + _x, -1 + _y);
	InitNewAliveCell(20 + _x,  _y);

	InitNewAliveCell(21 + _x, -2 + _y);
	InitNewAliveCell(21 + _x, -1 + _y);
	InitNewAliveCell(21 + _x, _y);

	InitNewAliveCell(22 + _x, -3 + _y);
	InitNewAliveCell(22 + _x, 1 + _y);

	InitNewAliveCell(24 + _x, -4 + _y);
	InitNewAliveCell(24 + _x, -3 + _y);
	InitNewAliveCell(24 + _x, 1 + _y);
	InitNewAliveCell(24 + _x, 2 + _y);

}

void Game::InitPrimordialSoup()
{
	const int _x = grid->GetLength();
	const int _y = grid->GetLength();
	const u_int& _numberOfNewCell = (_x * _y) / 5;
	for (u_int _index = 0; _index < _numberOfNewCell; _index++)
	{
		InitNewAliveCell(Random::GetRandomNumberInRange(0, _x - 1), Random::GetRandomNumberInRange(0, _y - 1));
	}
}

/// <summary>
/// Permet d'initialiser une nouvelle cellulle vivante
/// </summary>
/// <param name="_x"></param>
/// <param name="_y"></param>
void Game::InitNewAliveCell(const int _x, const int _y)
{
	Tile* _tempTile = grid->GetTile(_x, _y);
	_tempTile->UpdateNeighbourCount(3);
	PushCoordinateCellAlive(_tempTile->GetCoordinate());
}
