#include "TimeStampOnMap.h"

TimeStampOnMap::TimeStampOnMap()
{
}

TimeStampOnMap::~TimeStampOnMap()
{
}

void TimeStampOnMap::createTimeStampIterationOnMap(Grid & grid, const std::string & path, int iterations)
{
	struct StartEnd
	{
		CellIndex start;
		CellIndex end;
	};
	StartEnd* randomStartEndCells = new StartEnd[iterations];

	// Set all start and end nodes for all interations
	for (int i = 0; i < iterations; i++)
	{
		StartEnd se;
		// set start node
		do {
			int x = ((float)rand() / (float)RAND_MAX) * (grid.width - 1);
			int y = ((float)rand() / (float)RAND_MAX) * (grid.height - 1);
			CellIndex start;
			start.x = x;
			start.y = y;
			se.start = start;
		} while (grid.cells[se.start.x][se.start.y].type != eGround);

		// set end node
		do {
			int x = ((float)rand() / (float)RAND_MAX) * (grid.width - 1);
			int y = ((float)rand() / (float)RAND_MAX) * (grid.height - 1);
			CellIndex end;
			end.x = x;
			end.y = y;
			se.end = end;
		} while (grid.cells[se.end.x][se.end.y].type != eGround && !(se.end.x == se.start.x && se.end.y == se.start.y));

		randomStartEndCells[i] = se;
	}

	std::vector<double> tAStar;
	std::vector<double> tDirBasedAStar;

	std::chrono::high_resolution_clock clock;
	// Iterate and calcutate paths
	for (int i = 0; i < iterations; i++)
	{
		auto oldTime = clock.now();
		PathFinding::computeDirectionBasedAStart(randomStartEndCells[i].start, randomStartEndCells[i].end, grid);
		tDirBasedAStar.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(clock.now() - oldTime).count());
		oldTime = clock.now();
		PathFinding::computeAStart(randomStartEndCells[i].start, randomStartEndCells[i].end, grid);
		tAStar.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(clock.now() - oldTime).count());
	}

	std::string filePath = path + "_iterations" + std::to_string(iterations) + ".txt";
	std::ofstream file(filePath);
	if (file.is_open())
	{
		// A*
		file << "A* time stamp:\n";
		int i = 0;
		for (double time : tAStar)
		{
			file << time /*<< " ms, start [" << randomStartEndCells[i].start.x << "; " << randomStartEndCells[i].start.y << "], end [" << 
				randomStartEndCells[i].end.x << "; " << randomStartEndCells[i].end.y << "]" */<< "\n";
			i++;
		}

		// Direction Based A*
		file << "\nDirection Based A* time stamp:\n";
		i = 0;
		for (double time : tDirBasedAStar)
		{
			file << time /*<< " ms, start [" << randomStartEndCells[i].start.x << "; " << randomStartEndCells[i].start.y << "], end [" <<
				randomStartEndCells[i].end.x << "; " << randomStartEndCells[i].end.y << "]" */<< "\n";
			i++;
		}

		file.close();
	}
	delete randomStartEndCells;
}
