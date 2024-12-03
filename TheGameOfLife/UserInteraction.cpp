#include "UserInteraction.h"

pair<int, int> ChooseInputAndRetrieveCoords(const u_int& _optionsCount, pair<int, int> _pairOfIndexes)
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
			break;
		case 27: // Echap
			return { -1,-1 };
		case 75: // Fleche Haut <-
			_pairOfIndexes.first = (_pairOfIndexes.first <= 0 ? _optionsCount : _pairOfIndexes.first - 1);
			break;
		case 77: // Fleche Bas ->
			_pairOfIndexes.first = (_pairOfIndexes.first >= _optionsCount ? 0 : _pairOfIndexes.first + 1);
			break;
		case 72: // Fleche Haut ↑
			_pairOfIndexes.second = (_pairOfIndexes.second <= 0 ? _optionsCount : _pairOfIndexes.second - 1);
			break;
		case 80: // Fleche Bas ↓
			_pairOfIndexes.second = (_pairOfIndexes.second >= _optionsCount ? 0 : _pairOfIndexes.second + 1);
			break;
		
		default:
			break;
		}
	}
	return _pairOfIndexes;

}



void DisplayMenu(const string* _options, const int& _indexToSelect, const u_int& _optionsCount, const string& _question)
{
	string _firstSymbol = "[";
	string _secondSymbol = "]";

	DISPLAY("============================================================ Actions ============================================================", true);
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
	DISPLAY("\n=================================================================================================================================", true);
}
