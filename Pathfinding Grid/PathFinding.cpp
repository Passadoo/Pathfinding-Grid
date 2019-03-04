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
	bool** closed = new bool*[grid.size];
	for (int i = 0; i < grid.size; i++)
	{
		closed[i] = new bool[grid.size];
		for (int j = 0; j < grid.size; j++)
		{
			closed[i][j] = false;
			grid.cells[i][j].dir = eNONE;
			grid.cells[i][j].parentX = -1;
			grid.cells[i][j].parentY = -1;
		}
	}

	// Function to delete closed list
	auto releaseClosed = [closed, grid]() {
		for (int i = 0; i < grid.size; i++)
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

	// Change dir to 
	auto changeKey = [=](Grid grid, int x, int y)
	{
		Cell& cell = grid.cells[x][y];
		if (cell.dir == eLEFT) { cell.dir = eRIGHT; return; }
		if (cell.dir == eRIGHT) { cell.dir = eLEFT; return; }
		if (cell.dir == eUP) { cell.dir = eDOWN; return; }
		if (cell.dir == eDOWN) { cell.dir = eUP; return; }
		if (cell.dir == eRIGHT_DOWN) { cell.dir = eLEFT_UP; return; }
		if (cell.dir == eLEFT_UP) { cell.dir = eRIGHT_DOWN; return; }
		if (cell.dir == eLEFT_DOWN) { cell.dir = eRIGHT_UP; return; }
		if (cell.dir == eRIGHT_UP) { cell.dir = eLEFT_DOWN; return; }
	};

	// Init open list
	std::vector<Cell> open;

	// Add the start node
	open.push_back(grid.cells[startIndex.x][startIndex.y]);
	open.back().dir = eNONE;
	open.back().parentX = startIndex.x;
	open.back().parentY = startIndex.y;

	bool destinationFound = false;

	int x;
	int y;

	Cell& goal = grid.cells[endIndex.x][endIndex.y];
	Cell cell;

	while (!open.empty())
	{
		// Get the cell that is closest to the goal by looking at position and direction
		x = startIndex.x;
		y = startIndex.y;
		Path::iterator itCell;
		for (Path::iterator it = open.begin(); it != open.end(); it = next(it)) {
			Cell n = (*it);
			if (x == startIndex.x && y == startIndex.y)
			{
				x = n.xindex;
				y = n.yindex;
				itCell = it;
			}
			Dir dir = grid.cells[n.xindex][n.yindex].dir;
			if (dir == eNONE)
			{
				setDir(endIndex.x, endIndex.y, n.xindex, n.yindex, grid);
				dir = grid.cells[n.xindex][n.yindex].dir;
			}
			bool set = false;
			switch (dir)
			{
			case eLEFT: if (n.xindex < x) set = true; break;
			case eLEFT_UP: if (n.xindex < x && n.yindex < y) set = true; break;
			case eUP: if (n.yindex < y) set = true; break;
			case eRIGHT_UP: if (n.xindex > x && n.yindex < y) set = true; break;
			case eRIGHT: if (n.xindex > x) set = true; break;
			case eRIGHT_DOWN: if (n.xindex > x && n.yindex > y) set = true; break;
			case eDOWN: if (n.yindex > y) set = true; break;
			case eLEFT_DOWN: if (n.xindex < x && n.yindex > y) set = true; break;
			default:
				break;
			}
			if (set)
			{
				x = n.xindex;
				y = n.yindex;
				itCell = it;
			}
		}
		cell = *itCell;
		open.erase(itCell);

		closed[x][y] = true;

		struct hvd {
			int h = 0;
			int v = 0;
			Dir dir = eNONE;
			hvd(int h, int v, Dir dir)
			{
				this->h = h;
				this->v = v;
				this->dir = dir;
			};
		};

		int h = 0;
		int v = 0;
		if (endIndex.x < x) { h = -1; }
		if (endIndex.x > x) { h = 1; }
		if (endIndex.y < y) { v = -1; }
		if (endIndex.y > y) { v = 1; }

		std::vector<hvd> directions;
		if (cell.dir == eRIGHT)directions.push_back(hvd(h, v, eRIGHT));
		if (cell.dir == eLEFT)directions.push_back(hvd(h, v, eLEFT));
		if (cell.dir == eDOWN)directions.push_back(hvd(h, v, eDOWN));
		if (cell.dir == eUP)directions.push_back(hvd(h, v, eUP));
		if (h == 1 && v == 1) directions.push_back(hvd(h, v, eRIGHT)); directions.push_back(hvd(h, v, eDOWN));
		if (h == -1 && v == -1) directions.push_back(hvd(h, v, eLEFT)); directions.push_back(hvd(h, v, eUP));
		if (h == -1 && v == 1) directions.push_back(hvd(h, v, eLEFT)); directions.push_back(hvd(h, v, eDOWN));
		if (h == 1 && v == -1) directions.push_back(hvd(h, v, eRIGHT)); directions.push_back(hvd(h, v, eUP));

		int notfree = 0;
		for (hvd& hvd : directions)
		{
			if (isValid(x + hvd.h, y + hvd.v, grid))
			{
				if (isDestination(x + hvd.h, y + hvd.v, goal))
				{
					//Destination found - make path
					grid.cells[x + hvd.h][y + hvd.v].parentX = x;
					grid.cells[x + hvd.h][y + hvd.v].parentY = y;
					destinationFound = true;
					releaseClosed();
					return makePath(grid, goal, startIndex);
				}
				else if (closed[x + hvd.h][y + hvd.v] == false)
				{
					// Update the details of this neighbour node
					grid.cells[x + hvd.h][y + hvd.v].parentX = x;
					grid.cells[x + hvd.h][y + hvd.v].parentY = y;
					open.emplace_back(grid.cells[x + hvd.h][y + hvd.v]);
				}
			}
			else
				notfree++;
		}
		// None side is free, change dir
		if (notfree == directions.size())
		{
			changeKey(grid, x, y);
			open.emplace_back(grid.cells[x][y]);
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
	bool** closed = new bool*[grid.size];
	for (int i = 0; i < grid.size; i++)
	{
		closed[i] = new bool[grid.size];
		for (int j = 0; j < grid.size; j++)
		{
			closed[i][j] = false;
			grid.cells[i][j].f = FLT_MAX;
			grid.cells[i][j].g = FLT_MAX;
			grid.cells[i][j].h = FLT_MAX;
			grid.cells[i][j].parentX = -1;
			grid.cells[i][j].parentY = -1;
		}
	}

	// Add the start node
	open.push_back(grid.cells[startIndex.x][startIndex.y]);
	open.back().f = 0;
	open.back().g = 0;
	open.back().h = 0;
	open.back().parentX = startIndex.x;
	open.back().parentY = startIndex.y;

	bool destinationFound = false;

	int x;
	int y;

	Cell& dest = grid.cells[endIndex.x][endIndex.y];

	while (!open.empty())
	{
		Cell cell;
		// Get the cell with the smallest f value in the open list
		//do {
			float temp = FLT_MAX;
			Path::iterator itCell;
			for (Path::iterator it = open.begin();
				it != open.end(); it = next(it)) {
				Cell n = (*it);
				if (n.f < temp) {
					temp = n.f;
					itCell = it;
				}
			}
			cell = *itCell;
			open.erase(itCell);
		//} while (!open .empty() && !isValid(cell.xindex, cell.yindex, grid));

		x = cell.xindex;
		y = cell.yindex;
		closed[x][y] = true;

		//For each neighbour starting from North-West to South-East
		for (int newX = -1; newX <= 1; newX++) {
			for (int newY = -1; newY <= 1; newY++) {
				double gNew, hNew, fNew;
				if (isValid(x + newX, y + newY, grid)) {
					if (isDestination(x + newX, y + newY, dest))
					{
						//Destination found - make path
						grid.cells[x + newX][y + newY].parentX = x;
						grid.cells[x + newX][y + newY].parentY = y;
						destinationFound = true;

						for (int i = 0; i < grid.size; i++)
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
							open.emplace_back(grid.cells[x + newX][y + newY]);
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < grid.size; i++)
	{
		delete[] closed[i];
	}

	delete[] closed;

	std::cout << "Destination not found" << std::endl;
	return Path();
}
