#include "pch.h"
#include "UserInteraction.h"
#include <string>
#include <conio.h>

/* =============== Intéraction Utilisateur =============== */
void UserInteraction::DisplayMenu(const string* _options, const int _currentIndex, const u_int& _optionsCount, const string& _question)
{
	system("cls");
	DISPLAY("=============== Actions ===============", true);
	for (u_int _index = 0; _index < _optionsCount; _index++)
	{
		DISPLAY(_index + 1 << " -\t" << _options[_index], true);
	}
	DISPLAY("=======================================", true);
}

int UserInteraction::OpenMenu(const string* _options, const u_int& _optionsCount, const string& _question)
{
	u_int _currentIndex = 0;
	DisplayMenu(_options, _currentIndex, _optionsCount, _question);
	do
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
				return _currentIndex;
			case 72: // Fleche Haut ↑
				_currentIndex = (_currentIndex <= 0 ? _optionsCount : _currentIndex - 1);
				break;
			case 80: // Fleche Bas ↓
				_currentIndex = (_currentIndex >= _optionsCount ? 0 : _currentIndex + 1);
				break;
			default:
				break;
			}
			system("cls");
			DisplayMenu(_options, _currentIndex, _optionsCount, _question);
		}

	} while (true);
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

void UserInteraction::Sleep(const milliseconds& _duration)
{
	const auto _start = high_resolution_clock::now();
	sleep_for(_duration);
	const auto _end = high_resolution_clock::now();
	const duration<double, milli> _elapsed = _end - _start;
}