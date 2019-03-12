#include "PathFinding.h"
#include <queue>
#include <tuple>
#include <functional>

PathFinding::PathFinding()
{
}

PathFinding::~PathFinding()
{
}

Path PathFinding::computeDirectionBasedAStart(CellIndex startIndex, CellIndex endIndex, Grid grid)
{
	// Init closed list
	bool** closed = new bool*[grid.width];
	for (int i = 0; i < grid.width; i++)
	{
		closed[i] = new bool[grid.height];
		for (int j = 0; j < grid.height; j++)
		{
			closed[i][j] = false;
			grid.cells[i][j].dir = eNONE;
			grid.cells[i][j].parentX = -1;
			grid.cells[i][j].parentY = -1;

			grid.cells[i][j].debugType = eNoList;
		}
	}

	// Function to delete closed list
	auto releaseClosed = [closed, grid]() {
		for (int i = 0; i < grid.height; i++)
		{
			delete[] closed[i];
		}

		delete[] closed;
	};

	// Function to set direction of a cell
	auto setDir = [=](int goalx, int goaly, int x, int y, Grid grid) {
		int h = 0;
		int v = 0;
		if (goalx < x) { h = -1; grid.cells[x][y].dir = Dir::eLEFT; }
		if (goalx > x) {h = 1; grid.cells[x][y].dir = Dir::eRIGHT; }
		if (goaly < y) {v = -1; grid.cells[x][y].dir = Dir::eUP; }
		if (goaly > y) {v = 1; grid.cells[x][y].dir = Dir::eDOWN; }
		if (h == 1 && v == 1) grid.cells[x][y].dir = Dir::eRIGHT_DOWN;
		if (h == -1 && v == -1) grid.cells[x][y].dir = Dir::eLEFT_UP;
		if (h == -1 && v == 1) grid.cells[x][y].dir = Dir::eLEFT_DOWN;
		if (h == 1 && v == -1) grid.cells[x][y].dir = Dir::eRIGHT_UP;
	};

	auto check = [=](int oldx, int oldy, int x, int y, Grid grid) -> bool
	{
		bool checked = false;
		Cell& cell = grid.cells[x][y];
		switch (cell.dir)
		{
		case eLEFT:
			if (x <= oldx)
				checked = true;
			break;
		case eRIGHT:
			if (x >= oldx)
				checked = true;
			break;
		case eUP:
			if (y <= oldy)
				checked = true;
			break;
		case eDOWN:
			if (y >= oldy)
				checked = true;
			break;

		case eLEFT_UP:
			if (x <= oldx && y <= oldy)
				checked = true;
			break;
		case eRIGHT_UP:
			if (x >= oldx && y <= oldy)
				checked = true;
			break;
		case eRIGHT_DOWN:
			if (x >= oldx && y >= oldy)
				checked = true;
			break;
		case eLEFT_DOWN:
			if (x <= oldx && y >= oldy)
				checked = true;
			break;
		default:
			std::cout << "dir = eNONE" << std::endl;
			break;
		}

		return checked;
	};

	// Init open list
	std::vector<Cell> open;

	// Add the start node
	open.push_back(grid.cells[startIndex.x][startIndex.y]);
	open.back().parentX = startIndex.x;
	open.back().parentY = startIndex.y;
	open.back().debugType = eOpen;
	setDir(endIndex.x, endIndex.y, open.back().xindex, open.back().yindex, grid);

	int x = startIndex.x;
	int y = startIndex.y;

	Cell& goal = grid.cells[endIndex.x][endIndex.y];
	Cell cell;

	while (!open.empty())
	{
		// Test with only direction
		// Get the cell that is closest to the goal by looking at position and direction
		Path::iterator itCell = open.begin();
		bool first = false;
		for (Path::iterator it = open.begin(); it != open.end(); it = next(it)) {
			Cell n = (*it);
			if (check(x, y, n.xindex, n.yindex, grid) || !first)
			{
				first = true;
				x = n.xindex;
				y = n.yindex;
				itCell = it;
			}
		}

		cell = grid.cells[x][y];
		open.erase(itCell);

		grid.cells[x][y].debugType = eClosed;
		closed[x][y] = true;

		for (int newX = -1; newX <= 1; newX++) {
			for (int newY = -1; newY <= 1; newY++) {
				if (!(newX == 0 && newY == 0))
					if (isValid(x + newX, y + newY, grid)) {
						if (isDestination(x + newX, y + newY, goal))
						{
							//Destination found - make path
							grid.cells[x + newX][y + newY].parentX = x;
							grid.cells[x + newX][y + newY].parentY = y;
							releaseClosed();
							return makePath(grid, goal, startIndex);
						}
						else if (closed[x + newX][y + newY] == false)
						{
							if (grid.cells[x + newX][y + newY].dir == eNONE)
							{
								// Set dir
								setDir(endIndex.x, endIndex.y, x + newX, y + newY, grid);
								// Update the details of this neighbour node
								grid.cells[x + newX][y + newY].parentX = x;
								grid.cells[x + newX][y + newY].parentY = y;
								grid.cells[x + newX][y + newY].debugType = eOpen;
								open.emplace_back(grid.cells[x + newX][y + newY]);
							}
						}
					}
			}
		}
	}

	releaseClosed();

	std::cout << "Destination not found" << std::endl;
	return Path();
}

Path PathFinding::computeAStart(CellIndex startIndex, CellIndex endIndex, Grid grid)
{
	// Initialize both open and closed list
	std::vector<Cell> open;
	bool** closed = new bool*[grid.width];
	for (int i = 0; i < grid.width; i++)
	{
		closed[i] = new bool[grid.height];
		for (int j = 0; j < grid.height; j++)
		{
			closed[i][j] = false;
			grid.cells[i][j].f = FLT_MAX;
			grid.cells[i][j].g = FLT_MAX;
			grid.cells[i][j].h = FLT_MAX;
			grid.cells[i][j].parentX = -1;
			grid.cells[i][j].parentY = -1;
			grid.cells[i][j].debugType = eNoList;
		}
	}

	// Add the start node
	open.push_back(grid.cells[startIndex.x][startIndex.y]);
	open.back().f = 0;
	open.back().g = 0;
	open.back().h = 0;
	open.back().parentX = startIndex.x;
	open.back().parentY = startIndex.y;
	open.back().debugType = eOpen;

	int x;
	int y;

	Cell& dest = grid.cells[endIndex.x][endIndex.y];

	while (!open.empty())
	{
		Cell cell;
		float temp = FLT_MAX;
		float temph = FLT_MAX;
		Path::iterator itCell;
		for (Path::iterator it = open.begin();
			it != open.end(); it = next(it)) {
			Cell n = (*it);
			if (n.f == temp? n.h < temph : n.f < temp) {
				temp = n.f;
				temph = n.h;
				itCell = it;
			}
		}
		cell = *itCell;
		open.erase(itCell);

		x = cell.xindex;
		y = cell.yindex;
		closed[x][y] = true;
		grid.cells[x][y].debugType = eClosed;

		//For each neighbour starting from North-West to South-East
		for (int newX = -1; newX <= 1; newX++) {
			for (int newY = -1; newY <= 1; newY++) {
				float gNew, hNew, fNew;
				if (isValid(x + newX, y + newY, grid)) {
					if (isDestination(x + newX, y + newY, dest))
					{
						//Destination found - make path
						grid.cells[x + newX][y + newY].parentX = x;
						grid.cells[x + newX][y + newY].parentY = y;

						for (int i = 0; i < grid.height; i++)
						{
							delete[] closed[i];
						}

						delete[] closed;

						return makePath(grid, dest, startIndex);
					}
					else if (closed[x + newX][y + newY] == false)
					{
						gNew = cell.g + 1.0;
						hNew = calculateH(x + newX, y + newY, dest);
						fNew = gNew + hNew;
						// Check if this path is better than the one already present
						if (grid.cells[x + newX][y + newY].f == FLT_MAX || 
							grid.cells[x + newX][y + newY].f > fNew)
						{
							// Update the details of this neighbour node
							grid.cells[x + newX][y + newY].f = fNew;
							grid.cells[x + newX][y + newY].g = gNew;
							grid.cells[x + newX][y + newY].h = hNew;
							grid.cells[x + newX][y + newY].parentX = x;
							grid.cells[x + newX][y + newY].parentY = y;
							grid.cells[x + newX][y + newY].debugType = eOpen;
							open.emplace_back(grid.cells[x + newX][y + newY]);
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < grid.height; i++)
	{
		delete[] closed[i];
	}

	delete[] closed;

	std::cout << "Destination not found" << std::endl;
	return Path();
}
