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
	IT_E = 101,
	IT_F = 102,
	IT_G = 103,
	IT_L = 108,
	IT_K = 107,
	IT_J = 106,
	IT_R = 114,
	IT_DEBUG = 253,

	IT_COUNT,
};

enum MainActionType
{
	MAT_PLAY = 0,
	MAT_QUIT,
};

enum ActionType
{
	AT_GRID = 0,
	AT_SOUP,
	AT_GLIDER,
	AT_GLIDER_GUN,
	AT_BLOCK,
	AT_CLEAR,

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
	u_int generationCount;

	int isDebug;
	int isGrid;
	int isFr;

	int speed;
	
public:
	Game() = default;
	Game(const int _width, const int _length);
	~Game();
private:

	template<template<typename, typename>class Container, typename Type, typename Allocator = Type>
	void PushInVectorUnique(Container<Type, Allocator>& _container, const Type& _newItem)
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

	void Clear();


	/* ========== Algorithme ========== */

	void ResetWeightCell();
	void AddPointToNeighbourCell();
	void PushCoordinateCellAlive(Coordinate* _coordinate);
	void PushCoordinateCellAlive(const Coordinate& _coordinate);
	Coordinate ComputeNewCoordinate(Coordinate* _currentCoordinate, const Coordinate& _direction);
	void CheckCellAlive();
	bool IsAlreadyAlive(Coordinate _coordinateToCheck);
	void InitNewAliveCell(const int _x, const int _y);

	/* ========== Menus =========== */

	pair<int, int> ChooseInputAndRetrieveCoords(const int _optionsCount, pair<int, int> _pairOfIndexes);
	void DisplayMenu(const string* _options, const int& _indexToSelect, const u_int& _optionsCount, const string& _question);
	bool MainMenu();
	void OptionMenu();
	void GridMenu();
	int ChooseInputFromMainMenuAndRetrieveIndex(const int _optionsCount, int _currentIndex);
	int ChooseInputFromOptionMenuAndRetrieveIndex(const int _optionsCount, int _currentIndex);
	bool ChooseMainMenu(const int _menuIndex);
	bool ChooseOptionMenu(const int _menuIndex);
	
	/* ========== Display ========== */

	void DisplayCell(const vector<Coordinate*>& _cellCoordinates, const bool _unDisplay = false) const;
	void DisplayCursor(const pair<int, int>& _pairOfIndexes, const pair<int, int>& _oldPairOfIndexes) const;
	void DisplayInfos() const;

public:
	/* ========== Gameplay ========== */

	void InitPrefabByFile(const string& _filePath, const int _posX, const int _posY);

	void Loop();


	/* ========== Prefab ========== */

	void SavePrefab(const string& _name);
	void InitPrimordialSoup();
	
};

