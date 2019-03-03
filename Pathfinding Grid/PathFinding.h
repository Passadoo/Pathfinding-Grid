#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

#include "Helper.h"
#include <iostream>
#include <stack>

class PathFinding
{
private:


public:
	PathFinding();
	~PathFinding();

	static Path computeAStart(CellIndex startIndex, CellIndex endIndex, Grid grid);

	static bool isValid(int x, int y, Grid grid) { //If our Node is an obstacle it is not valid
		int id = x + y * grid.size;
		if (x < 0 || y < 0 || x >= grid.size || y >= grid.size) {
			return false;
		}
		if (grid.getCell(id).type != CellType::eObstacle) {
			return true;
		}
		return false;
	}

	static bool isDestination(int x, int y, Cell dest) {
		if (x == dest.xindex && y == dest.yindex) {
			return true;
		}
		return false;
	}

	static double calculateH(int x, int y, Cell dest) {
		double H = (sqrt((x - dest.xindex)*(x - dest.xindex)
			+ (y - dest.yindex)*(y - dest.yindex)));
		return H;
	}

	static Path makePath(Grid grid, Cell dest, CellIndex startIndex) {
		std::cout << "Found a path" << std::endl;
		Path usablePath;

		bool found = false;
		int x = dest.parentX;
		int y = dest.parentY;
		while (!found)
		{
			usablePath.insert(usablePath.begin(), grid.cells[x][y]);
			Cell& t = grid.cells[x][y];
			x = t.parentX;
			y = t.parentY;
			t.type = CellType::ePath;

			if (usablePath[0].parentX == startIndex.x && usablePath[0].parentY == startIndex.y)
				found = true;
		}

		return usablePath;
	}
};