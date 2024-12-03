#include "pch.h"
#include "Macro.h"

GAMEOFLIFELIB__API void Macro::Config()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	SetConsoleOutputCP(CP_UTF8);
	locale::global(locale("fr_FR"));
}

/* =============== System =============== */
void System::Sleep(const milliseconds& _duration)
{
	const auto _start = high_resolution_clock::now();
	sleep_for(_duration);
	const auto _end = high_resolution_clock::now();
	const duration<double, milli> _elapsed = _end - _start;
}

void System::MaximizeConsole()
{
	HWND Hwnd = GetForegroundWindow();
	int x = GetSystemMetrics(SM_CXSCREEN);
	int y = GetSystemMetrics(SM_CYSCREEN);
	LONG winstyle = GetWindowLong(Hwnd, GWL_STYLE);
	SetWindowLong(Hwnd, GWL_STYLE, (winstyle | WS_POPUP | WS_MAXIMIZE) & ~WS_CAPTION & ~WS_THICKFRAME & ~WS_BORDER);
	SetWindowPos(Hwnd, HWND_TOP, 0, 0, x, y, 0);
}


/* =============== Intéraction Utilisateur =============== */
void UserInteraction::DisplayMenu(const string* _options, const pair<int, int>& _pairOfIndexes, const u_int& _optionsCount, const string& _question)
{
	string _firstSymbol = "[";
	string _secondSymbol = "]";
	
	DISPLAY("============================== Actions ==============================", true);
	for (u_int _index = 0; _index < _optionsCount; _index++)
	{
		if (_pairOfIndexes.first != _index && _pairOfIndexes.second != _index)
		{
			_firstSymbol = "";
			_secondSymbol = "";
		}
		else
		{
			_firstSymbol = "[";
			_secondSymbol = "]";
		}

		DISPLAY(_firstSymbol << _options[_index] << _secondSymbol << "\t\t", false);
	}
	DISPLAY("\n====================================================================", true);
}

pair<int, int> UserInteraction::OpenMenu(const string* _options, const u_int& _optionsCount, const string& _question, pair<int, int> _pairOfIndexes)
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

int UserInteraction::GetInputByUser()
{
	int _key;
	if (_kbhit())
	{
		_key = _getch();
		return _key;
	}
	return 0;
}


/* =============== Random =============== */
int Random::GetRandomNumberInRange(const int _min, const int _max)
{
	random_device _rSeed; // Obtient un graine aléatoire
	mt19937 _gen(_rSeed()); // Initialise le générateur avec la graine
	uniform_int_distribution<> _distr(_min, _max); // Définit la distribution (les limites)

	return _distr(_gen); // Génération du nombre
}
