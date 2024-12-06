#include "Game.h"
#include "UserInteraction.h"
#include "windows.h"

using namespace Stream;

Game::Game(const int _width, const int _length)
{
	hwnd = GetForegroundWindow();
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	cursorInfo = new CONSOLE_CURSOR_INFO();
	cursorInfo->dwSize = 100;
	cursorInfo->bVisible = FALSE;

	grid = new Grid(_width, _length);

	isDebug = 0;
	isGrid = 0;
	isFr = 0;

	generationCount = 0;
	speed = 100;

	inputType = IT_COUNT;
}

Game::~Game()
{
	delete grid;
	delete cursorInfo;
}


/* ========== Gameplay ========== */ 

void Game::NextIteration()
{
	generationCount++;
	AddPointToNeighbourCell();
	CheckCellAlive();
	DisplayCell(oldCoordinatesCellAlive, true);
}

void Game::AutoIteration()
{
	while (true)
	{
		int _input = 0;
		if (speed < 0) speed = 1;
		DisplayCell(coordinatesCellAlive);

		if (_kbhit())
		{
			_input = _getch();
			if (_input == IT_J) speed += 20;
			else if (_input == IT_K)  speed -= 5;
			else if (_input == IT_F) break;
			
		}

		NextIteration();
	}


	//do
	//{
	//	_input = 0;

	//	if (_input == IT_J) speed += 20;
	//	else if (_input == IT_K) speed -= 20;
	//	else if (speed < 0) speed = 0;

	//	DisplayCell(coordinatesCellAlive);
	//	Sleep(speed);
	//	NextIteration();
	//} while (_input == IT_K || _input == IT_J || !_kbhit());
	
	
}

void Game::Clear()
{
	for (Coordinate* _element : coordinatesCellAlive)
	{
		grid->GetTile(_element)->RemoveCell();
	}
	CheckCellAlive();
}

void Game::InitPrefabByFile(const string& _prefabName, const int _posX, const int _posY)
{
	string _paternName;
	u_int _length;
	u_int _width;
	
	ifstream _stream = ifstream("Pattern\\" + _prefabName + ".txt");
	
	if (!_stream)
	{
			cerr << "ERROR => the file : " << _prefabName << " doesn't exist !" << endl;
			return;
	}

	_stream >> _paternName;
	_stream >> _length;
	_stream >> _width;
	delete grid;
	grid = new Grid(_length, _width);
	coordinatesCellAlive = oldCoordinatesCellAlive = vector<Coordinate*>();
	int _lineIndex = -1;
	string _line;
	while (getline(_stream, _line))
	{
		if (_lineIndex != -1)
		{
			u_int _lineSize = static_cast<u_int>(_line.size());
			int _binary;
			for (u_int _index = 0; _index < _lineSize; _index++)
			{
				_binary = _line[_index] - '0';
				if (_binary == 1)InitNewAliveCell(_index, _lineIndex);
			}
		}
		++_lineIndex;
	}
}

void Game::Loop()
{
	bool _isOver = false;
	do
	{
		_isOver = MainMenu();
	} while (!_isOver);
}


/* ========== Algorithme ========== */

void Game::ResetWeightCell()
{
	const u_int& _coordinatesCellAliveSize = (u_int)coordinatesCellAlive.size();

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

	const u_int& _coordinatesCellAliveSize = (u_int)coordinatesCellAlive.size();

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
		}
	}
	DisplayCell(oldCoordinatesCellAlive, true);
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
		case IT_E:
			NextIteration();
			break;
		case IT_F:
			AutoIteration();
			break;
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
	string _color = RESET;

	for (u_int _index = 0; _index < _optionsCount; _index++)
	{
		if (_indexToSelect != _index)
		{
			_firstSymbol = " ";
			_secondSymbol = " ";
			_color = GRAY;
		}
		else
		{
			_firstSymbol = "[";
			_secondSymbol = "]";
			_color = YELLOW;
		}

		DISPLAY(_firstSymbol << _color << _options[_index] << RESET << _secondSymbol << "   ", false);
	}
}

bool Game::MainMenu()
{
	const u_int& _actionsCount = 2;
	string* _actions = nullptr;
	
	int _index = 0;
	bool _isOver = false;
	do
	{
		StreamSystem _it;

		if (isFr)
		{
			delete[] _actions;
			_it = StreamSystem("", "MainMenuAscii_Fr", "txt", ios_base::in);

			_actions = new string[_actionsCount]
			{
				"Jouer",
				"Quitter"
			};
		}
		else 
		{ 
			delete[] _actions;
			_it = StreamSystem("", "MainMenuAscii_En", "txt", ios_base::in); 

			_actions = new string[_actionsCount]
			{
				"Play",
				"Quit"
			};
		}

		SetConsoleCursorInfo(consoleHandle, cursorInfo);

		SetConsoleCursorPosition(consoleHandle, { 0,0 });
		DISPLAY(BLINK_TEXT, false);
		_it.DisplayByRow(40, 0);
		DISPLAY(RESET, false);
		
		_index = ChooseInputFromMainMenuAndRetrieveIndex(_actionsCount - 1, _index);

		SetConsoleCursorPosition(consoleHandle, { 95,45 });
		DisplayMenu(_actions, _index, _actionsCount, "");

		if (isFr) 
		{
			DISPLAY("\nFait Par VT Et Sacha", true);
		}
		else 
		{
			DISPLAY("\nMade By VT And Sacha", true);
		}
		
		if (_index == -1) _isOver = true;
		
	} while (!_isOver);

	delete[] _actions;
	return _isOver;
}

void Game::OptionMenu()
{
	const u_int& _actionsCount = 7;
	string* _actions = nullptr;

	int _index = 0;
	bool _isOver = false;
	do
	{
		if (!isFr)
		{
			delete[] _actions;
			_actions = new string[_actionsCount]
			{
				"Show Grid",
				"Primordial Soup",
				"Glider",
				"Glider Gun",
				"Block",
				"Clear Grid",
				"Quit"
			};
		}
		else
		{
			delete[] _actions;
			_actions = new string[_actionsCount]
			{
				"Afficher la Grille",
				"Soupe Primordiale",
				"Glider",
				"Canon à Glider",
				"Bloc",
				"Clear la Grille",
				"Quitter"
			};
		}

		_index = ChooseInputFromOptionMenuAndRetrieveIndex(_actionsCount - 1, _index);

		SetConsoleCursorPosition(consoleHandle, { 60,20 });
		SetConsoleCursorInfo(consoleHandle, cursorInfo);

		DisplayMenu(_actions, _index, _actionsCount, "");

		DisplayInfos();

		if (_index == -1) _isOver = true;

	} while (!_isOver);

	delete[] _actions;
}

void Game::GridMenu()
{
	pair<int, int> _pairOfIndexes = make_pair(0, 0);
	pair<int, int> _oldPairOfIndexes;

	grid->Display(isGrid, _pairOfIndexes);
	bool _isOver = false;
	do
	{
		_oldPairOfIndexes = _pairOfIndexes;
		
		SetConsoleCursorPosition(consoleHandle, { 0,0 });
		_pairOfIndexes = ChooseInputAndRetrieveCoords(grid->GetLength(), _pairOfIndexes);

		// Quitter
		if (_pairOfIndexes.first == -1 && _pairOfIndexes.second == -1) _isOver = true;

		// Colors
		if (_pairOfIndexes.first == -2 && _pairOfIndexes.second == -2) _isOver = true;

		DisplayCursor(_pairOfIndexes, _oldPairOfIndexes);

		DisplayCell(coordinatesCellAlive, false);

		CheckCellAlive();

	} while (!_isOver);

	system("cls");
}

int Game::ChooseInputFromMainMenuAndRetrieveIndex(const int _optionsCount, int _currentIndex)
{
	bool _wantToReturn = false;
	if (_kbhit())
	{
		u_int _input = 0;
		_input = _getch();

		switch (_input)
		{
		case IT_ENTER:
			_wantToReturn = ChooseMainMenu(_currentIndex);
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

int Game::ChooseInputFromOptionMenuAndRetrieveIndex(const int _optionsCount, int _currentIndex)
{
	bool _wantToReturn = false;
	if (_kbhit())
	{
		u_int _input = 0;
		_input = _getch();

		switch (_input)
		{
		case IT_ENTER:
			_wantToReturn = ChooseOptionMenu(_currentIndex);
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
		case IT_L:
			// Toggle Language
			++isFr %= 2;
			return -1;
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
	case MAT_PLAY:
		system("cls");
		OptionMenu();
		system("cls");
		break;
	case MAT_QUIT:
		return true;
	}
	return false;
}

bool Game::ChooseOptionMenu(const int _menuIndex)
{
	system("cls");
	switch (_menuIndex)
	{
	case AT_GRID:
		GridMenu();
		break;
	case AT_SOUP:
		InitPrimordialSoup();
		break;
	case AT_GLIDER:
		InitPrefabByFile("Glider", 5, 5);
		break;
	case AT_GLIDER_GUN:
		InitPrefabByFile("Gun", 5, 5);
		break;
	case AT_BLOCK:
		InitPrefabByFile("Block", 5, 5);
		break;
	case AT_CLEAR:
		Clear();
		break;
	case AT_QUIT:
		return true;
	}
	return false;
}




/* ========== Prefab ========== */

void Game::SavePrefab(const string& _name)
{
	fstream _ofstream = fstream("Pattern\\PrefabNames.txt", ios::app);
	_ofstream << _name + "\n";

	fstream _newFile = fstream("Pattern\\" + _name + ".txt", ios::app);

	string _parameter;
	_parameter += _name + " " + to_string(grid->GetLength()) + " " + to_string(grid->GetWidth());
	_newFile << _parameter + "\n";

	auto _yPriority = [](Coordinate* _firstCoordinate, Coordinate* _secondCoordinate) { return _firstCoordinate->y < _secondCoordinate->y; };
	sort(coordinatesCellAlive.begin(), coordinatesCellAlive.end(), _yPriority);

	const u_int _lastY = static_cast<int>(coordinatesCellAlive[coordinatesCellAlive.size() - 1]->y);
	const u_int _coordinateCellAliveSize = static_cast<int>(coordinatesCellAlive.size());

	int _occurence = 0;
	int _indexLine = 0;
	for (u_int _inedx = 0; _inedx < _lastY + 1; _inedx++)
	{
		do
		{
			++_occurence;
		} while ((_indexLine + _occurence) != _coordinateCellAliveSize && (coordinatesCellAlive[_indexLine]->y == coordinatesCellAlive[_indexLine + _occurence]->y));

		auto _xPriority = [](Coordinate* _firstCoordinate, Coordinate* _secondCoordinate) { return _firstCoordinate->x < _secondCoordinate->x; };
		sort(coordinatesCellAlive.begin() + _indexLine, coordinatesCellAlive.begin() + _indexLine + _occurence, _xPriority);

		string _line;

		int _start0 = 0;
		for (int _index = _indexLine; _index < _indexLine + _occurence; _index++)
		{
			int _limitOf0 = coordinatesCellAlive[_index]->x;
			for (int _inedx = _start0; _inedx < _limitOf0 - 1; _inedx++)
			{
				_line += "0";
			}
			_line += "1";
			_start0 = coordinatesCellAlive[_index]->x;

		}
		_newFile << _line + "\n";

		_indexLine += _occurence;
		_occurence = 0;

	}

}

void Game::InitPrimordialSoup()
{
	const int _x = grid->GetWidth();
	const int _y = grid->GetLength();
	const u_int& _numberOfNewCell = (_x * _y) / 2;
	for (u_int _index = 0; _index < _numberOfNewCell; _index++)
	{
		InitNewAliveCell(Random::GetRandomNumberInRange(_x / 5, _x - _x / 5), Random::GetRandomNumberInRange(_y / 5, _y - _y / 5));
	}
}




/* ========== Display ========== */
 
void Game::DisplayCell(const vector<Coordinate*>& _cellCoordinates, const bool _unDisplay) const
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
	if (!_unDisplay) Sleep(speed);
}

void Game::DisplayCursor(const pair<int, int>& _pairOfIndexes, const pair<int, int>& _oldPairOfIndexes) const
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

void Game::DisplayInfos() const
{
	for (u_int _index = 0; _index < 12; _index++)
	{
		DISPLAY("", true);
	}
	if (isFr)
	{
		DISPLAY("Nombre de Génération : " + to_string(generationCount), true);
		DISPLAY("Vitesse Actuel (en millisecondes) : " + to_string(speed), true);

		DISPLAY("\n\nRaccourcis du Menu :\n", true);
		DISPLAY("Appuyer sur 'L' pour Changer la Langue\t\tAppuyer sur 'G' pour Afficher la Grille\n", true);

		DISPLAY("\n\nRaccourcis de la Grille :\n", true);
		DISPLAY("Appuyer sur 'E' pour la Génération Suivante", true);
		DISPLAY("\nAppuyer sur 'F' pour le Mode Auto", true);
		DISPLAY("\nAppuyer sur 'J' lors du Mode Auto pour Réduire la Vitesse\nAppuyer sur 'K' lors du Mode Auto pour Augmenter la Vitesse", true);
		DISPLAY("\n\nAppuyer sur 'Echap' pour Quitter\tAppuyer sur 'Enter' pour Sélectionner une Cellule", false);
	}
	else
	{
		DISPLAY("Generation Count : " + to_string(generationCount), true);
		DISPLAY("Current Speed (in milliseconds) : " + to_string(speed), true);

		DISPLAY("\n\nMenu Hotkeys :\n", true);
		DISPLAY("Press 'L' to Change Language\tPress 'G' to Display Grid", true);

		DISPLAY("\n\nGrid Hotkeys :\n", true);
		DISPLAY("Press 'E' to Next Generation", true);
		DISPLAY("\nPress 'F' to Auto Mode", true);
		DISPLAY("\nPress 'J' while in Auto Mode to Decrease Speed\ntPress 'K' while in Auto Mode to Increase Speed", true);
		DISPLAY("\n\nPress 'Echap' to Quit\t\t\tPress 'Enter' to Select Cell", false);
	}	
}