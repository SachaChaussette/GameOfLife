#pragma once

#ifdef GAMEOFLIFELIB_EXPORTS
#define GAMEOFLIFELIB__API __declspec(dllexport)
#else
#define GAMEOFLIFELIB__API __declspec(dllimport)
#endif

#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <random>
#include <conio.h>
#include <thread>
#include "Color.h"
#include <vector>

#pragma region Define

#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define DISPLAY(_value, _isEndl)    cout << _value;\
                                    if (_isEndl) cout << endl;
#define CLEAR  cin.clear();\
               cin.ignore((numeric_limits<streamsize>::max)(), '\n');

#pragma endregion

using namespace std;
using namespace std::chrono_literals;
using namespace chrono;
using namespace this_thread;

#pragma region TypeDef

typedef unsigned int u_int;
typedef const unsigned int c_u_int;

#pragma endregion

namespace Macro
{
	GAMEOFLIFELIB__API void Config();
};

namespace System
{
	/// <summary>
	/// /!\ ATTENTION FONCTIONS BLOQUANTE /!\ Stop le programme pendant _duration millisecondes 
	/// </summary>
	/// <param name="_duration"></param>
	GAMEOFLIFELIB__API void Sleep(const milliseconds& _duration);

	GAMEOFLIFELIB__API void MaximizeConsole();
};

namespace UserInteraction
{

	/// <summary>
	/// Affiche une liste de string proprement avec l'index sur le coté
	/// </summary>
	/// <param name="_actionsArray"></param>
	GAMEOFLIFELIB__API void DisplayMenu(const string* _options, const int _currentIndex, const u_int& _optionsCount, const string& _question);

	GAMEOFLIFELIB__API int OpenMenu(const string* _options, const u_int& _optionsCount, const string& _question);
	/// <summary>
	/// Retourne l'input de l'utilisateur si il presse une touche
	/// </summary>
	/// <returns></returns>
	GAMEOFLIFELIB__API int GetInputByUser();


	/// <summary>
	/// Retourne un type de donnée donné par l'utilisateur avec une question
	/// </summary>
	/// <param name="_question"></param>
	/// <returns></returns>
	template<typename ReturnType>
	ReturnType Retrieve(const string& _question)
	{
		DISPLAY(_question, true);
		ReturnType _result;
		cin >> _result;
		CLEAR;
		return _result;
	}
}

namespace Random
{
	/// <summary>
	/// Retourne un nombre aléatoire en _min et _max
	/// </summary>
	/// <param name="_min"></param>
	/// <param name="_max"></param>
	/// <returns></returns>
	GAMEOFLIFELIB__API int GetRandomNumberInRange(const int _min, const int _max);
}


