#include "Game.h"
#include "UserInteraction.h"
#include "windows.h"

Game::Game()
{
	hwnd = GetForegroundWindow();
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	cursorInfo->dwSize = 100;
	cursorInfo->bVisible = FALSE;


	grid = new Grid();
	iterationCount = 0;
}

Game::Game(const int _width, const int _length)
{
	hwnd = GetForegroundWindow();
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	cursorInfo = new CONSOLE_CURSOR_INFO();
	cursorInfo->dwSize = 100;
	cursorInfo->bVisible = FALSE;

	grid = new Grid(_width, _length);
	iterationCount = 0;
}

Game::~Game()
{
	delete grid;
	delete cursorInfo;
}


/* ========== Gameplay ========== */ 

void Game::NextIteration()
{
	iterationCount++;
	AddPointToNeighbourCell();
	CheckCellAlive();
	DisplayCell(oldCoordinatesCellAlive, true);

}

void Game::AutoIteration()
{
	while (!_kbhit())
	{
		DisplayCell(coordinatesCellAlive);
		NextIteration();
	}
}

bool Game::IsOver()
{
	// TODO ISOVER
	return true;
}

void Game::Loop()
{
	InitGun(5,5);
	do
	{
		SelectionMenu();
	} while (!IsOver());
	
	
	// TODO LOOP
}


/* ========== Algorithme ========== */

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
	for (u_int _index = 0; _index < _garbageSize; _index++)
	{
		PushCoordinateCellAlive(_garbageCoord[_index]);
		if (!IsAlreadyAlive(_garbageCoord[_index]))
		{
			grid->GetTile(_garbageCoord[_index])->ResetNeigbourCount();
		}
	}

}

void Game::PushCoordinateCellAlive(Coordinate* _coordinate)
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

void Game::PushCoordinateCellAlive(const Coordinate& _coordinate)
{
	Tile* _tileToUpdate = grid->GetTile(_coordinate);
	CellState _currentCellState = _tileToUpdate->UpdateCellState();
	Coordinate* _coordinateToModify = _tileToUpdate->GetCoordinate();
	if (_currentCellState == CT_ALIVE)
	{
		u_int _coordinatesCellAliveSize = (u_int)coordinatesCellAlive.size();
		for (u_int _index = 0; _index < _coordinatesCellAliveSize; _index++)
		{
			if (coordinatesCellAlive[_index] == _coordinateToModify) return;
		}
		coordinatesCellAlive.push_back(_coordinateToModify);
		sort(coordinatesCellAlive.begin(), coordinatesCellAlive.end());
	}
	else
	{
		u_int _coordinatesCellAliveSize = (u_int)coordinatesCellAlive.size();
		for (u_int _index = 0; _index < _coordinatesCellAliveSize; _index++)
		{
			if (coordinatesCellAlive[_index] == _coordinateToModify)
			{
				coordinatesCellAlive.erase(coordinatesCellAlive.begin() + _index);
				return;
			}
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
			coordinatesCellAlive.erase(coordinatesCellAlive.begin() + _index);
			_coordinatesCellAliveSize = (u_int)coordinatesCellAlive.size();
			--_index;
			DisplayCell(oldCoordinatesCellAlive, true);

		}
	}
}

bool Game::IsAlreadyAlive(Coordinate _coordinateToCheck)
{
	const u_int& _coordinatesCellAliveCount = static_cast<const u_int&>(coordinatesCellAlive.size());
	for (u_int _index = 0; _index < _coordinatesCellAliveCount; _index++)
	{
		if (_coordinateToCheck == coordinatesCellAlive[_index]) return true;
	}
	return false;
}


/* ========== Menus =========== */

pair<int, int> Game::ChooseInputAndRetrieveCoords(const int _optionsCount, pair<int, int> _pairOfIndexes)
{
	if (_kbhit())
	{
		// Attendre une touche
		u_int _input = 0;
		_input = _getch();

		Coordinate* _coordinateToTest;
		Tile* _tempTile;

		switch (_input)
		{
		case IT_ENTER: // Entrer
			_coordinateToTest = grid->GetTile(_pairOfIndexes.second, _pairOfIndexes.first)->GetCoordinate();
			_tempTile = grid->GetTile(_coordinateToTest);
			if (!IsAlreadyAlive(*_coordinateToTest))
			{
				_tempTile->UpdateNeighbourCount(3);
				PushCoordinateCellAlive(_coordinateToTest);
			}
			else
			{
				_tempTile->RemoveCell();
			}
			
			break;
		case IT_ECHAP:
			return { -1,-1 };
		case IT_LEFT:
			_pairOfIndexes.second = (_pairOfIndexes.second <= 0 ? _optionsCount - 1 : _pairOfIndexes.second - 1);
			break;
		case IT_RIGHT:
			_pairOfIndexes.second = (_pairOfIndexes.second >= _optionsCount - 1 ? 0 : _pairOfIndexes.second + 1);
			break;
		case IT_UP:
			_pairOfIndexes.first = (_pairOfIndexes.first <= 0 ? _optionsCount - 1 : _pairOfIndexes.first - 1);
			break;
		case IT_DOWN:
			_pairOfIndexes.first = (_pairOfIndexes.first >= _optionsCount - 1 ? 0 : _pairOfIndexes.first + 1);
			break;
		case IT_A:
			// TODO Previous Iteration
			break;
		case IT_E:
			NextIteration();
			break;
		case IT_F:
			AutoIteration();
			break;
		case IT_G:
			return { -3,-3 };
		case IT_DEBUG:
			return { -2,-2 };
		

		default:
			break;
		}
	}
	return _pairOfIndexes;

}

void Game::DisplayMenu(const string* _options, const int& _indexToSelect, const u_int& _optionsCount, const string& _question)
{
	string _firstSymbol = "[";
	string _secondSymbol = "]";

	DISPLAY("======== Actions ========", true);
	for (u_int _index = 0; _index < _optionsCount; _index++)
	{
		if (_indexToSelect != _index)
		{
			_firstSymbol = " ";
			_secondSymbol = " ";
		}
		else
		{
			_firstSymbol = "[";
			_secondSymbol = "]";
		}

		DISPLAY(_firstSymbol << _options[_index] << _secondSymbol << "\t", false);
	}
	DISPLAY("\n=========================", true);
}

void Game::SelectionMenu()
{
	string _actions[2]
	{
		"Choisir une Cellule à Modifier",
		"Quitter"
	};
	const u_int& _actionsCount = sizeof(_actions) / sizeof(string);

	int _index = 0;
	
	do
	{
		SetConsoleCursorPosition(consoleHandle, { 0,0 });
		SetConsoleCursorInfo(consoleHandle, cursorInfo);

		_index = ChooseInputAndRetrieveIndex(_actionsCount - 1, _index);
		DisplayMenu(_actions, _index, _actionsCount, "");

		if (_index == -1) return;
		
	} while (true);

}

void Game::GridMenu()
{
	pair<int, int> _pairOfIndexes = make_pair(0, 0);
	// TODO Update : ancienne coordonnée pour effacer les anciens curseur
	pair<int, int> _oldPairOfIndexes;
	// TODO Update : La grille se Display qu'une seule fois
	grid->Display(isGrid, _pairOfIndexes);
	do
	{
		_oldPairOfIndexes = _pairOfIndexes;
		
		SetConsoleCursorPosition(consoleHandle, { 0,0 });
		_pairOfIndexes = ChooseInputAndRetrieveCoords(grid->GetLength(), _pairOfIndexes);

		// Quitter
		if (_pairOfIndexes.first == -1 && _pairOfIndexes.second == -1) return;

		// TODO Update : Le curseur s'affiche à part de la grid
		DisplayCursor(_pairOfIndexes, _oldPairOfIndexes);

		DisplayCell(coordinatesCellAlive, false);

		CheckCellAlive();
		//DisplayInfos();

	} while (true);
}

int Game::ChooseInputAndRetrieveIndex(const int _optionsCount, int _currentIndex)
{
	bool _wantToReturn = false;
	if (_kbhit())
	{
		// Attendre une touche
		u_int _input = 0;
		_input = _getch();

		switch (_input)
		{
		case IT_ENTER: // Entrer
			_wantToReturn = ChooseMainMenu(_currentIndex);
			if (_wantToReturn) return -1;
			break;
		case IT_LEFT:
			_currentIndex = (_currentIndex <= 0 ? _optionsCount : _currentIndex - 1);
			break;
		case IT_RIGHT:
			_currentIndex = (_currentIndex >= _optionsCount ? 0 : _currentIndex + 1);
			break;
		case IT_G:
			// Toggle Grid
			++isGrid %= 2;
			break;
		case IT_DEBUG:
			// Toggle Debug
			++isDebug %= 2;
			break;
		default:
			break;
		}
	}
	return _currentIndex;

}

bool Game::ChooseMainMenu(const int _menuIndex)
{
	switch (_menuIndex)
	{
	case AT_GRID:
		system("cls");
		GridMenu();
		system("cls");
		break;
	case AT_QUIT:
		return true;
	}
	return false;
}


/* ========== Prefab ========== */

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
	InitNewAliveCell(20 + _x, _y);

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


/* ========== Display ========== */
 
void Game::DisplayCell(const vector<Coordinate*>& _cellCoordinates, const bool _unDisplay)
{
	u_int _coordinatesCellAliveSize = (u_int)_cellCoordinates.size();
	for (u_int _index = 0; _index < _coordinatesCellAliveSize; _index++)
	{
		Coordinate* _coordinate = _cellCoordinates[_index];
		if (isGrid)
		{
			SetConsoleCursorPosition(consoleHandle, { static_cast<short>(_coordinate->x * 5 + 2), static_cast<short>(_coordinate->y * 2+ 1) });
		}
		else
		{
			SetConsoleCursorPosition(consoleHandle, { static_cast<short>(_coordinate->x * 2), static_cast<short>(_coordinate->y) });
		}
		if (_unDisplay)
		{
			if (binary_search(coordinatesCellAlive.begin(), coordinatesCellAlive.end(), _cellCoordinates[_index])) continue;
			DISPLAY(string(RESET) + "  ", false);
		}
		else
		{
			grid->GetTile(_coordinate)->Display(isDebug);
		}
	}
	if (!_unDisplay) Sleep(100);
}

void Game::DisplayCursor(const pair<int, int>& _pairOfIndexes, const pair<int, int>& _oldPairOfIndexes)
{
	for (int _rowIndex = 0; _rowIndex < int(grid->GetLength()); _rowIndex++)
	{
		for (int _colIndex = 0; _colIndex < int(grid->GetWidth()); _colIndex++)
		{
			if (_pairOfIndexes.first == _colIndex && _pairOfIndexes.second == _rowIndex && isGrid)
			{
				
				SetConsoleCursorPosition(consoleHandle, { static_cast<short>(_pairOfIndexes.second * 5 + 1), static_cast<short>(_pairOfIndexes.first * 2 + 1)});
				DISPLAY("[  ]", false);
				if (_pairOfIndexes != _oldPairOfIndexes)
				{
					SetConsoleCursorPosition(consoleHandle, { static_cast<short>(_oldPairOfIndexes.second * 5 + 1), static_cast<short>(_oldPairOfIndexes.first * 2 + 1) });
					DISPLAY("    ", false);
				}
				return;
				
			}
			else if (_pairOfIndexes.first == _colIndex && _pairOfIndexes.second == _rowIndex)
			{
				DISPLAY(BG_DARK_GRAY, false);
				SetConsoleCursorPosition(consoleHandle, { static_cast<short>(_pairOfIndexes.second * 2), static_cast<short>(_pairOfIndexes.first) });
				DISPLAY("  ", false);
				DISPLAY(RESET, false);
				if (_pairOfIndexes != _oldPairOfIndexes)
				{
					SetConsoleCursorPosition(consoleHandle, { static_cast<short>(_oldPairOfIndexes.second * 2), static_cast<short>(_oldPairOfIndexes.first) });
					DISPLAY("  ", false);
				}
				return;
			}
		}
	}
}

void Game::DisplayInfos()
{
	DISPLAY("\n\n\n\n\n\nNombre d'Itérations : " + to_string(iterationCount), true);

	DISPLAY("\nPress 'A' to Previous Iteration\t\tPress 'E' to Next Iteration", true);
	DISPLAY("\nPress 'F' to Auto Mode\t\t\tPress 'Enter' to Select Cell", true);
	DISPLAY("\n\nPress 'Echap' to Quit\t", true);
}