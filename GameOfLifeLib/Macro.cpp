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
