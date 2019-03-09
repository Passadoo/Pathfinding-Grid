#include "MapLoader.h"

MapLoader::MapLoader()
{
}

MapLoader::~MapLoader()
{
}

void MapLoader::loadMap(Grid & grid, const std::string & path)
{
	sf::Texture map;
	map.loadFromFile(path);
	grid.width = map.getSize().x;
	grid.height = map.getSize().y;

	grid.cells = new Cell*[grid.width];

	for (int i = 0; i < grid.width; i++)
	{
		grid.cells[i] = new Cell[grid.height];
	}

	sf::Image image = map.copyToImage();
	sf::Color color;
	//Fill the array with ground tiles
	for (int i = 0; i < grid.width; i++)
	{
		for (int j = 0; j < grid.height; j++)
		{
			color = image.getPixel(i, j);
			grid.cells[i][j].xindex = i;
			grid.cells[i][j].yindex = j;
			grid.cells[i][j].position = sf::Vector2f(grid.cellSize * i, grid.cellSize * j);
			if (color == sf::Color(0, 0, 0, 255)) // Black
				grid.cells[i][j].type = eObstacle;
			else
				grid.cells[i][j].type = eGround;
		}
	}
}

void MapLoader::setZoomFactor(Grid & grid)
{
	int x = grid.width - 1;
	float zoomFactorX = (sf::VideoMode::getDesktopMode().width - grid.cells[x][0].position.x) / -x;
	int y = grid.height - 1;
	float zoomFactorY = (sf::VideoMode::getDesktopMode().height - grid.cells[0][y].position.y) / -y;

	float zoomFactor = (zoomFactorX > zoomFactorY? zoomFactorX: zoomFactorY);
	grid.cellSize = grid.cellSize - zoomFactor;

	for (int i = 0; i < grid.width; i++)
	{
		for (int j = 0; j < grid.height; j++)
		{
			grid.cells[i][j].position = sf::Vector2f(grid.cells[i][j].position.x - i * zoomFactor, grid.cells[i][j].position.y - j * zoomFactor);
		}
	}
}
