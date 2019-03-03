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

	/*const int width = grid.size;
	auto idx = [width](int x, int y) {
		return x + y * width;
	};

	auto h = [=](int u) -> int { // lower bound distance to target from u
		int x = u % width, y = u / width;
		int dx = std::abs(x - endIndex.x);
		int dy = std::abs(y - endIndex.y);
		return 10 * (dx + dy) + (14 - 2 * 10) * std::min(dx, dy);
	};

	const int n = grid.size * grid.size;
	const int startPos = idx(startIndex.x, startIndex.y), targetPos = idx(endIndex.x, endIndex.y);

	int discovered = 0;
	std::vector<int> p(n), d(n, INT_MAX);
	std::priority_queue<std::tuple<int, int, int>,
		std::vector<std::tuple<int, int, int>>, 
		std::greater<std::tuple<int, int, int>>> pq; // A* with tie breaking
	d[startPos] = 0;
	pq.push(std::make_tuple(0 + h(startPos), 0, startPos));
	while (!pq.empty())
	{
		int u = std::get<2>(pq.top());
		pq.pop();
		for (auto e : { +1, -1, +grid.size, -grid.size, -grid.size - 1, -grid.size + 1, +grid.size - 1, +grid.size + 1 })
		{
			int v = u + e;
			if (((e == 1 || e == -grid.size + 1 || e == grid.size + 1) && (v % grid.size == 0))
				|| ((e == -1 || e == -grid.size - 1 || e == grid.size - 1) && (u % grid.size == 0)))
				continue;
			if (0 <= v && v < n && d[v] > d[u] + 1 && grid.isGround(v)) {
				p[v] = u;
				d[v] = d[u] + 1;
				if (v == targetPos)
					goto end;
				pq.push(std::make_tuple(d[v] + h(v), ++discovered, v));
			}
		}
	}
end:

	if (d[targetPos] == INT_MAX) {
		return Path();
	}
	else
	{

		Path path;
		int curr = targetPos;
		for (int i = d[targetPos] - 1; i >= 0; i--) {
			grid.getCell(curr).type = CellType::ePath;
			path.insert(path.begin(), grid.getCell(curr));
			curr = p[curr];
		}
		//return d[targetPos];
		return path;
	}*/
}
