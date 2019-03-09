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
	eOpen,
	eClosed,
	eNoList,
	eEnd
};

enum Dir 
{ 
	eNONE, 
	eLEFT, 
	eLEFT_UP, 
	eUP, 
	eRIGHT_UP, 
	eRIGHT, 
	eRIGHT_DOWN, 
	eDOWN, 
	eLEFT_DOWN 
};

struct Cell
{
	float f = FLT_MAX;
	float g = FLT_MAX;
	float h = FLT_MAX;
	int xindex = 0;
	int yindex = 0;
	Dir dir = eNONE;
	int parentX = -1;
	int parentY = -1;
	CellType type;
	CellType debugType = CellType::eNoList;
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
	int width = MAP_DIMENSION;
	int height = MAP_DIMENSION;
	float cellSize = START_SIZE_OF_CELLS;
	Cell** cells;

	void cleanUp() 
	{
		for (int i = 0; i < height; i++)
		{
			delete[] cells[i];
		}

		delete[] cells;
	};

	Cell& getCell(int cellIndex)
	{
		int x = cellIndex % width;
		int y = (int)std::floor(cellIndex / height);
		return cells[x][y];
	};

	bool isGround(int cellIndex)
	{
		return getCell(cellIndex).type == CellType::eGround;
	}
};