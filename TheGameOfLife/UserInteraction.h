#pragma once

#include "..\GameOfLifeLib\Macro.h"

pair<int, int> ChooseInputAndRetrieveCoords(const u_int& _optionsCount, pair<int, int> _pairOfIndexes);
int ChooseInputAndRetrieveIndex(const u_int& _optionsCount, int _currentIndex);

void DisplayMenu(const string* _options, const int& _indexToSelect, const u_int& _optionsCount, const string& _question);
