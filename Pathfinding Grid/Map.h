#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#include "Helper.h"
#include "PathFinding.h"

class Map
{
private:
	//Movement and Zoom
	const float ZOOM_FACTOR = 2.0f;
	const int MOVEMENT_FACTOR = 150;

	sf::Texture mGroundTexture;
	sf::Texture mObstacleTexture;
	sf::Texture mStartTexture;
	sf::Texture mEndTexture;
	sf::Sprite mSprite;

	Grid mGrid;
	Path path;
	CellIndex start;
	CellIndex end;
	Cell* startCellOld = nullptr;
	Cell* endCellOld = nullptr;
	int hasStartEnd = 0; // 0 = false, 1 = start, 2 = end
	bool mlDown = false;
	bool mrDown = false;
	bool XDown = false;
	bool DirBasedPathFinding = false;

	bool mColorBool = true;

public:
	Map();
	~Map();

	void update(float dt);
	void ProcessInput(sf::RenderWindow& window);
	void Draw(sf::RenderWindow& window, sf::RenderStates states = sf::RenderStates::Default);
};

