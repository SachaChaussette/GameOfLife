#pragma once

#include "Grid.h"

class Game
{
	HWND hwnd;
	HANDLE consoleHandle;
	CONSOLE_CURSOR_INFO cursorInfo;
	Grid* grid;
	vector<Coordinate*> coordinatesCellAlive;
	vector<Coordinate*> oldCoordinatesCellAlive;

public:
	inline bool IsOver()
	{
		return coordinatesCellAlive.size() == 0;
	}

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
	void AddPointToNeighbourCell();
	void CheckCellAlive();
	Coordinate ComputeNewCoordinate(Coordinate* _currentCoordinate, const Coordinate& _direction);
	void PushCoordinateCellAlive(Coordinate* _coordinate);
	void PushCoordinateCellAlive(const Coordinate& _coordinate);
	void ResetWeightCell();
	bool NotInAliveCell(Coordinate _coordinateToCheck);
	void DisplayCell(const vector<Coordinate*>& _cellCoordinates, const bool _unDisplay = false);

public:
	void Gameloop();
	void InitGlider(const int _x = 0 , const int _y = 0);
	void InitBlock(const int _x = 0, const int _y = 0);
	void InitGunPart1(const int _x = 0, const int _y = 0);
	void InitGun(const int _x = 0 , const int _y = 0);
	void InitPrimordialSoup();
	void InitPatternByFile(const string& _path);
	void InitNewAliveCell(const int _x, const int _y);
};
