#pragma once

#include "Macro.h"
#include <thread>

using namespace std::chrono_literals;
using namespace chrono;
using namespace this_thread;

namespace UserInteraction
{
	template <typename ReturnType>
	GAMEOFLIFELIB__API ReturnType Retrieve(const string& _question);

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
	/// /!\ ATTENTION FONCTIONS BLOQUANTE /!\ Stop le programme pendant _duration millisecondes 
	/// </summary>
	/// <param name="_duration"></param>
	GAMEOFLIFELIB__API void Sleep(const milliseconds& _duration);
	

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

