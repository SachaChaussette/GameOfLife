#include "Game.h"
#include "UserInteraction.h"
#include "windows.h"

Game::Game()
{
	hwnd = GetForegroundWindow();
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	cursorInfo.dwSize = 100;
	cursorInfo.bVisible = FALSE;
	grid = new Grid();
	iterationCount = 0;
}

Game::Game(const int _width, const int _length)
{
	hwnd = GetForegroundWindow();
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	cursorInfo.dwSize = 100;
	cursorInfo.bVisible = FALSE;
	grid = new Grid(_width, _length);
	iterationCount = 0;
}

Game::~Game()
{
	delete grid;
}

void Game::NextIteration()
{
	iterationCount++;
	AddPointToNeighbourCell();
	CheckCellAlive();
	// TODO UPDATE TILESWITHCELL
}

bool Game::IsOver()
{
	// TODO ISOVER
	return true;
}

void Game::Loop()
{ 
	do
	{
		SelectionMenu();
	} while (!IsOver());
	
	
	// TODO LOOP
}

void Game::DisplayGrid(const bool _showGrid)
{
	//grid->Display(_showGrid,);
	DISPLAY("\n\n", true)
		Sleep(500);
}

void Game::DisplayInfos()
{
	DISPLAY("\n\nNombre d'Itérations : " + to_string(iterationCount), true);

	DISPLAY("\nPress 'A' to Previous Iteration\t\tPress 'E' to Next Iteration", true);
	DISPLAY("\nPress 'F' to Auto Mode\t\t\tPress 'Enter' to Select Cell", true);
	DISPLAY("\nPress 'G' to Enable/Disable Grid", true);
	DISPLAY("\n\nPress 'Echap' to Quit\t", true);
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
			coordinatesCellAlive.erase(coordinatesCellAlive.begin() + _index);
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
	for (u_int _index = 0; _index < _garbageSize; _index++)
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


/* ========== Menus =========== */
pair<int, int> Game::ChooseInputAndRetrieveCoords(const u_int& _optionsCount, pair<int, int> _pairOfIndexes)
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
			if (!IsAlreadyAlive(_pairOfIndexes))
			{
				_tempTile->UpdateNeighbourCount(3);
				PushCellAlive(_coordinateToTest);
			}
			else
			{
				CheckCellAlive();
				_tempTile->RemoveCell();
				CheckCellAlive();
			}
			
			break;
		case IT_ECHAP:
			return { -1,-1 };
		case IT_LEFT:
			_pairOfIndexes.first = (_pairOfIndexes.first <= 0 ? _optionsCount - 1 : _pairOfIndexes.first - 1);
			break;
		case IT_RIGHT:
			_pairOfIndexes.first = (_pairOfIndexes.first >= _optionsCount - 1 ? 0 : _pairOfIndexes.first + 1);
			break;
		case IT_UP:
			_pairOfIndexes.second = (_pairOfIndexes.second <= 0 ? _optionsCount - 1 : _pairOfIndexes.second - 1);
			break;
		case IT_DOWN:
			_pairOfIndexes.second = (_pairOfIndexes.second >= _optionsCount - 1 ? 0 : _pairOfIndexes.second + 1);
			break;
		case IT_A:
			// TODO Previous Iteration
			break;
		case IT_E:
			NextIteration();
			break;
		case IT_F:
			// TODO Auto Mode
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
		_index = ChooseInputAndRetrieveIndex(_actionsCount - 1, _index);
		DisplayMenu(_actions, _index, _actionsCount, "");
		if (_index == -1) return;
		


	} while (true);

}

void Game::GridMenu()
{
	pair<int, int> _pairOfIndexes = make_pair(0, 0);
	int _isDebug = 0;
	int _isGrid = 0;
	do
	{
		SetConsoleCursorPosition(consoleHandle, { 0,0 });
		SetConsoleCursorPosition(consoleHandle, { 0,0 });
		_pairOfIndexes = ChooseInputAndRetrieveCoords(grid->GetLength(), _pairOfIndexes);

		// Quitter
		if (_pairOfIndexes.first == -1 && _pairOfIndexes.second == -1) return;

		// Toggle Debug
		if (_pairOfIndexes.first == -2 && _pairOfIndexes.second == -2)
		{
			_pairOfIndexes = { 0,0 };
			++_isDebug %= 2;
		}
		// Toggle Grid
		if (_pairOfIndexes.first == -3 && _pairOfIndexes.second == -3)
		{
			system("cls");
			_pairOfIndexes = { 0,0 };
			++_isGrid %= 2;
		}
		grid->Display(_isGrid, _pairOfIndexes, _isDebug);
		DisplayInfos();

	} while (true);
}

int Game::ChooseInputAndRetrieveIndex(const u_int& _optionsCount, int _currentIndex)
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
			_wantToReturn = SelectionMenu(_currentIndex);
			if (_wantToReturn) return -1;
			break;
		case IT_LEFT:
			_currentIndex = (_currentIndex <= 0 ? _optionsCount : _currentIndex - 1);
			break;
		case IT_RIGHT:
			_currentIndex = (_currentIndex >= _optionsCount ? 0 : _currentIndex + 1);
			break;
		default:
			break;
		}
	}
	return _currentIndex;

}

bool Game::SelectionMenu(const int _menuIndex)
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

bool Game::IsAlreadyAlive(const pair<int, int>& _pairOfIndexes)
{
	const u_int& coordinatesCellAliveCount = coordinatesCellAlive.size();
	for (u_int _index = 0; _index < coordinatesCellAliveCount; _index++)
	{

		if (coordinatesCellAlive[_index]->y == _pairOfIndexes.first &&
			coordinatesCellAlive[_index]->x == _pairOfIndexes.second)
		{
			return true;
		}
	}
	return false;
}