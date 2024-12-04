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

void Game::Gameloop()
{
	SetConsoleCursorInfo(consoleHandle, &cursorInfo);

	InitGlider();
	do
	{
		SetConsoleCursorPosition(consoleHandle, {0, 0});
		DisplayGrid();
		/*grid->Display(false, true);*/
		AddPointToNeighbourCell();
		CheckCellAlive();
		

	} while (!IsOver());
}

bool Game::IsOver()
{
	return coordinatesCellAlive.size() == 0;
}

void Game::DisplayGrid(const bool _showGrid)
{
	grid->Display(_showGrid);
	DISPLAY("\n\n", true)
	Sleep(500);
}

void Game::CheckCellAlive()
{
	u_int coordinatesCellAliveSize = (u_int)coordinatesCellAlive.size();
	for (u_int _index = 0; _index < coordinatesCellAliveSize; _index++)
	{
		Coordinate* _coordinateToCheck = coordinatesCellAlive[_index];
		Tile* _tileToUpdate = grid->GetTile(_coordinateToCheck);
		if (_tileToUpdate->GetCellState() == CT_DEAD)
		{
 			_tileToUpdate->SetAppearance(RESET);
			coordinatesCellAlive.erase(coordinatesCellAlive.begin()+_index);
			coordinatesCellAliveSize = (u_int)coordinatesCellAlive.size();
			--_index;
		}
	}
}

void Game::ResetWeightCell()
{
	const u_int coordinatesCellAliveSize = (u_int)coordinatesCellAlive.size();
	for (u_int _index = 0; _index < coordinatesCellAliveSize; _index++)
	{
		Coordinate* _currentCoordinate = coordinatesCellAlive[_index];
		Tile* _tempTile = grid->GetTile(_currentCoordinate);
		_tempTile->SetWeight(0);
	}
}

void Game::InitGlider()
{
	Coordinate* _coordinateToTest = grid->GetTile(1, 2)->GetCoordinate();
	Tile* _tempTile = grid->GetTile(_coordinateToTest);
	_tempTile->UpdateNeighbourCount(3);
	PushCellAlive(_coordinateToTest);

	Coordinate* _coordinateToTest2 = grid->GetTile(2, 3)->GetCoordinate();
	Tile* _tempTile2 = grid->GetTile(_coordinateToTest2);
	_tempTile2->UpdateNeighbourCount(3);
	PushCellAlive(_coordinateToTest2);

	Coordinate* _coordinateToTest3 = grid->GetTile(3, 1)->GetCoordinate();
	Tile* _tempTile3 = grid->GetTile(_coordinateToTest3);
	_tempTile3->UpdateNeighbourCount(3);
	PushCellAlive(_coordinateToTest3);

	Coordinate* _coordinateToTest4 = grid->GetTile(3, 2)->GetCoordinate();
	Tile* _tempTile4 = grid->GetTile(_coordinateToTest4);
	_tempTile4->UpdateNeighbourCount(3);
	PushCellAlive(_coordinateToTest4);

	Coordinate* _coordinateToTest5 = grid->GetTile(3, 3)->GetCoordinate();
	Tile* _tempTile5 = grid->GetTile(_coordinateToTest5);
	_tempTile5->UpdateNeighbourCount(3);
	PushCellAlive(_coordinateToTest5);
}

/// <summary>
/// Fonction qui sert à debug
/// </summary>
void Game::AddPointToAll()
{
	u_int _length = grid->GetLength();
	u_int _width = grid->GetWidth();
	for (u_int _indexX = 0;  _indexX < _length;  _indexX++)
	{
		for (u_int _indexY = 0; _indexY < _width; _indexY++)
		{
			grid->GetTile( _indexX , _indexY)->UpdateNeighbourCount(1);
		}
	}
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
	const u_int coordinatesCellAliveSize = (u_int)coordinatesCellAlive.size();
	for (u_int _index = 0; _index < coordinatesCellAliveSize; _index++)
	{
		Coordinate* _currentCoordinate = coordinatesCellAlive[_index];
		for (Coordinate _direction : _directions)
		{
			const int _newX = (_currentCoordinate->x + _direction.x) < 0 ? 0 : (_currentCoordinate->x + _direction.x) != grid->GetLength() ? _currentCoordinate->x + _direction.x : grid->GetLength() - 1;
			const int _newY = (_currentCoordinate->y + _direction.y) < 0 ? 0 : (_currentCoordinate->y + _direction.y) != grid->GetWidth() ? _currentCoordinate->y + _direction.y : grid->GetLength() - 1;
			Coordinate _coordinateToChange = { _newX, _newY };
			Tile* _tileToUpdate = grid->GetTile(_coordinateToChange);
			_tileToUpdate->UpdateNeighbourCount(1);


			bool _isIn = false;
			const u_int& _garbageSize = (u_int)_garbageCoord.size();
			for (u_int _index = 0; _index < _garbageSize; _index++)
			{
				if (_garbageCoord[_index] == _coordinateToChange)
				{
					_isIn = true;
					break;
				}
			}
			if (!_isIn) _garbageCoord.push_back(_coordinateToChange);
		}
		
	}
	const u_int& _garbageSize = (u_int)_garbageCoord.size();
	for (u_int _index = 0; _index< _garbageSize; _index++)
	{
		PushCellAlive(_garbageCoord[_index]);
		grid->GetTile(_garbageCoord[_index])->SelfMutilate();
	}

}


void Game::PushCellAlive(Coordinate* _coordinate)
{
	Tile* _tileToUpdate = grid->GetTile(_coordinate);
	CellState _currentCellState = _tileToUpdate->UpdateCellState();
	if (_currentCellState == CT_ALIVE)
	{
		Coordinate* _coordinateToPush = _tileToUpdate->GetCoordinate();
		u_int coordinatesCellAliveSize = (u_int)coordinatesCellAlive.size();
		for (u_int _index = 0; _index < coordinatesCellAliveSize; _index++)
		{
			if (coordinatesCellAlive[_index] == _coordinateToPush) return;
		}
		coordinatesCellAlive.push_back(_coordinateToPush);
	}
}

void Game::PushCellAlive(const Coordinate& _coordinate)
{
	Tile* _tileToUpdate = grid->GetTile(_coordinate);
	CellState _currentCellState = _tileToUpdate->UpdateCellState();
	if (_currentCellState == CT_ALIVE)
	{
		Coordinate* _coordinateToPush = _tileToUpdate->GetCoordinate();
		u_int coordinatesCellAliveSize = (u_int)coordinatesCellAlive.size();
		for (u_int _index = 0; _index < coordinatesCellAliveSize; _index++)
		{
			if (coordinatesCellAlive[_index] == _coordinateToPush) return;
		}
		coordinatesCellAlive.push_back(_coordinateToPush);
	}
}
