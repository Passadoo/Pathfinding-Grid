#pragma once

#include "Helper.h"
#include "PathFinding.h"
#include <string>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <chrono>

class TimeStampOnMap
{
private:

public:
	TimeStampOnMap();
	~TimeStampOnMap();

	static void createTimeStampIterationOnMap(Grid& grid, const std::string& path, int iterations);
};