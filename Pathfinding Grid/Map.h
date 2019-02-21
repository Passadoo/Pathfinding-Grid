#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class Map
{
private:
	float OLD_CELL_SIZE = 20;
	float CELL_SIZE = 20;
	const int MAP_DIMENSION = 100;

	//Movement and Zoom
	const int ZOOM_FACTOR = 2;
	const int MOVEMENT_FACTOR = 150;

	sf::Texture mGroundTexture;
	sf::Texture mObstacleTexture;
	sf::Texture mStartTexture;
	sf::Texture mEndTexture;
	sf::Sprite mSprite;

	enum CellType
	{
		eGround = 0,
		eObstacle,
		eStart,
		eEnd
	};

	struct Cell
	{
		CellType type;
		sf::Vector2f position;
	};

	Cell** mCells;

	bool mColorBool = true;

public:
	Map();
	~Map();

	void update(float dt);
	void ProcessInput(sf::RenderWindow& window);
	void Draw(sf::RenderWindow& window, sf::RenderStates states = sf::RenderStates::Default);
};

