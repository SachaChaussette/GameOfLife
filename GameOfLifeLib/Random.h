#pragma once

#include "Macro.h"
#include <random>

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

