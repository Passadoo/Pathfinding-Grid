#pragma once

#include "Helper.h"
#include <string>
#include <SFML/Graphics.hpp>

class MapLoader
{
private:

public:
	MapLoader();
	~MapLoader();

	static void loadMap(Grid& grid, const std::string& path);
	static void setZoomFactor(Grid& grid);
};