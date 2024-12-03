#include "Game.h"
#include "UserInteraction.h"
#include "windows.h"

Game::Game(const Grid& _grid)
{
	grid = _grid;
	tilesWicthCell = vector<Tile>();
	iterationCount = 0;
}

void Game::NextIteration()
{
	iterationCount++;
	// TODO UPDATE TILESWITHCELL
}

void Game::GiveWeightToTile()
{
	// TODO GiveWeightToTile
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

void Game::SelectionMenu()
{
	string _actions[5]
	{
		"Choisir une Cellule à Modifier",
		"Itération Suivante",
		"Itération Précédente",
		"Mode Auto",
		"Quitter"
	};
	const u_int& _actionsCount = sizeof(_actions) / sizeof(string);

	int _index = 0;
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	do
	{

		
		_index = ChooseInputAndRetrieveIndex(_actionsCount - 1, _index);
		DisplayMenu(_actions, _index, _actionsCount, "Choisit une Action à faire.");
		SetConsoleCursorPosition(console, { 0,0 });

		
	} while (true);
	
}

void Game::GridMenu()
{
	pair<int, int> _pairOfIndexes = make_pair(0, 0);

	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	do
	{
		SetConsoleCursorPosition(console, { 0,0 });
		_pairOfIndexes = ChooseInputAndRetrieveCoords(grid.GetLength(), _pairOfIndexes);

		// Quitter
		if (_pairOfIndexes.first == -1 && _pairOfIndexes.second == -1) return;

		grid.Display(true, _pairOfIndexes);
	} while (true);
}

void Game::ChooseMenu(const int _menuIndex)
{
	switch (_menuIndex)
	{
	case 0:
		system("cls");
		GridMenu();
		system("cls");
		break;
	case 1:
		// TODO
		break;
	case 2:
		// TODO
		break;
	case 3:
		// TODO
		break;
	case 4:
		return;
	}
}

int Game::ChooseInputAndRetrieveIndex(const u_int& _optionsCount, int _currentIndex)
{
	if (_kbhit())
	{
		// Attendre une touche
		u_int _input = 0;
		_input = _getch();

		// Si la touche est entrée, alors _isChoiceMade = true
		switch (_input)
		{
		case 13: // Entrer
			ChooseMenu(_currentIndex);
			break;
		case 75: // Fleche Haut <-
			_currentIndex = (_currentIndex <= 0 ? _optionsCount : _currentIndex - 1);
			break;
		case 77: // Fleche Bas ->
			_currentIndex = (_currentIndex >= _optionsCount ? 0 : _currentIndex + 1);
			break;
		default:
			break;
		}
	}
	return _currentIndex;

}