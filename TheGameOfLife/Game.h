#pragma once

#include "Grid.h"

enum InputType
{
	IT_ENTER = 13,
	IT_ECHAP = 27,
	IT_UP = 122,
	IT_DOWN = 115,
	IT_LEFT = 113,
	IT_RIGHT = 100,
	IT_A = 97,
	IT_E = 101,
	IT_F = 102,
	IT_G = 103,
	IT_DEBUG = 253,

	IT_COUNT,
};

enum ActionType
{
	AT_GRID = 0,
	AT_QUIT,
};

class Game
{
	InputType inputType;

	HWND hwnd;
	HANDLE consoleHandle;
	CONSOLE_CURSOR_INFO* cursorInfo;

	Grid* grid;
	vector<Coordinate*> coordinatesCellAlive;
	vector<Coordinate*> oldCoordinatesCellAlive;
	u_int iterationCount;

	int isDebug;
	int isGrid;
public:
	Game();
	Game(const int _width, const int _length);
	~Game();
private:

	template<template<typename, typename>class Container, typename Type, typename Allocator = Type>
	void PushInVectorUnique(Container<Type, Allocator>& _container, Type _newItem)
	{
		bool _isIn = false;
		const u_int& _containerSize = (u_int)_container.size();
		for (u_int _index = 0; _index < _containerSize; _index++)
		{
			if (_container[_index] == _newItem)
			{
				_isIn = true;
				break;
			}
		}
		if (!_isIn) _container.push_back(_newItem);

	}


	/* ========== Gameplay ========== */

	void NextIteration();

	void AutoIteration();


	/* ========== Algorithme ========== */

	void ResetWeightCell();
	void AddPointToNeighbourCell();
	void PushCoordinateCellAlive(Coordinate* _coordinate);
	void PushCoordinateCellAlive(const Coordinate& _coordinate);
	Coordinate ComputeNewCoordinate(Coordinate* _currentCoordinate, const Coordinate& _direction);
	void CheckCellAlive();
	bool IsAlreadyAlive(Coordinate _coordinateToCheck);

	/* ========== Menus =========== */

	pair<int, int> ChooseInputAndRetrieveCoords(const int _optionsCount, pair<int, int> _pairOfIndexes);
	void DisplayMenu(const string* _options, const int& _indexToSelect, const u_int& _optionsCount, const string& _question);
	void SelectionMenu();
	void GridMenu();
	int ChooseInputAndRetrieveIndex(const int _optionsCount, int _currentIndex);
	bool ChooseMainMenu(const int _menuIndex);
	
	/* ========== Display ========== */

	void DisplayCell(const vector<Coordinate*>& _cellCoordinates, const bool _unDisplay = false);
	void DisplayCursor(const pair<int, int>& _pairOfIndexes, const pair<int, int>& _oldPairOfIndexes);

public:

	/* ========== Display ========== */

	void DisplayInfos();

	/* ========== Gameplay ========== */

	bool IsOver();
	void Loop();


	/* ========== Prefab ========== */

	void InitGlider(const int _x = 0, const int _y = 0);
	void InitBlock(const int _x = 0, const int _y = 0);
	void InitGunPart1(const int _x = 0, const int _y = 0);
	void InitGun(const int _x = 0, const int _y = 0);
	void InitPrimordialSoup();
	void InitNewAliveCell(const int _x, const int _y);
};

