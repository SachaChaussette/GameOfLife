#include "Game.h"

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
	return false;
}

void Game::Loop()
{
	string _actions[2]
	{
		"Choisir une Cellule à Modifier",
		"Pochaine Itération",
	};
	pair<int, int> _pairOfIndexes = make_pair(0,0);
	
	grid.Display(false);
	do
	{
		_pairOfIndexes = UserInteraction::OpenMenu(_actions, 2, "", _pairOfIndexes);

	} while (true);

	
	
	// TODO LOOP
}



pair<int, int> OpenMenu(const string* _options, const u_int& _optionsCount, const string& _question, pair<int, int> _pairOfIndexes)
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
			return _pairOfIndexes;
		case 75: // Fleche Haut <-
			_pairOfIndexes.second = (_pairOfIndexes.second <= 0 ? _optionsCount : _pairOfIndexes.second - 1);
			break;
		case 77: // Fleche Bas ->
			_pairOfIndexes.second = (_pairOfIndexes.second >= _optionsCount ? 0 : _pairOfIndexes.second + 1);
			break;
		case 72: // Fleche Haut ↑
			_pairOfIndexes.first = (_pairOfIndexes.first <= 0 ? _optionsCount : _pairOfIndexes.first - _optionsCount);
			break;
		case 80: // Fleche Bas ↓
			_pairOfIndexes.first = (_pairOfIndexes.first >= _optionsCount ? 0 : _pairOfIndexes.first + _optionsCount);
			break;
		default:
			break;
		}
		DisplayMenu(_options, _pairOfIndexes, _optionsCount, _question);
	}
	return _pairOfIndexes;

}