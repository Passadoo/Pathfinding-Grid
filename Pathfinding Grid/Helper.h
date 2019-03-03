#pragma once

#include <SFML/Graphics.hpp>

const int MAP_DIMENSION = 100;
const float START_SIZE_OF_CELLS = 20;

enum CellType
{
	eGround = 0,
	ePath,
	eObstacle,
	eStart,
	eEnd
};

struct Cell
{
	float f = FLT_MAX;
	float g = FLT_MAX;
	float h = FLT_MAX;
	int xindex = 0;
	int yindex = 0;
	int parentX = -1;
	int parentY = -1;
	CellType type;
	sf::Vector2f position;
};

typedef std::vector<Cell> Path;

struct CellIndex
{
	int x;
	int y;
};


struct Grid
{
	int size = MAP_DIMENSION;
	float cellSize = START_SIZE_OF_CELLS;
	float cellOldSize = START_SIZE_OF_CELLS;
	Cell** cells;

	void cleanUp() 
	{
		for (int i = 0; i < size; i++)
		{
			delete[] cells[i];
		}

		delete[] cells;
	};

	Cell& getCell(int cellIndex)
	{
		int x = cellIndex % size;
		int y = (int)std::floor(cellIndex / size);
		return cells[x][y];
	};

	bool isGround(int cellIndex)
	{
		return getCell(cellIndex).type == CellType::eGround;
	}
};