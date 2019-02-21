#include "Map.h"



Map::Map()
{

	mGroundTexture.loadFromFile("Assets/GroundTexture.png");
	mObstacleTexture.loadFromFile("Assets/ObstacleTexture.png");
	mStartTexture.loadFromFile("Assets/StartTexture.png");
	mEndTexture.loadFromFile("Assets/EndTexture.png");

	//mSprite.scale(0.25f, 0.25f);

	//Create the 2 dimensional array
	mCells = new Cell*[MAP_DIMENSION];

	for (int i = 0; i < MAP_DIMENSION; i++)
	{
		mCells[i] = new Cell[MAP_DIMENSION];
	}

	//Fill the array with ground tiles
	for (int i = 0; i < MAP_DIMENSION; i++)
	{
		for (int j = 0; j < MAP_DIMENSION; j++)
		{
			mCells[i][j].position = sf::Vector2f(CELL_SIZE * i, CELL_SIZE * j);
			mCells[i][j].type = eGround;
		}
	}

	std::cout << "Use Arrow keys to move" << std::endl;
	std::cout << "Use the K and L keys to Zoom" << std::endl;
}


Map::~Map()
{
	for (int i = 0; i < MAP_DIMENSION; i++)
	{
		delete[] mCells[i];
	}

	delete[] mCells;
}

void Map::update(float dt)
{
	//MOVE FACTOR
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		for (int i = 0; i < MAP_DIMENSION; i++)
		{
			for (int j = 0; j < MAP_DIMENSION; j++)
			{
				mCells[i][j].position = sf::Vector2f(mCells[i][j].position.x - MOVEMENT_FACTOR * dt, mCells[i][j].position.y);
			}
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		for (int i = 0; i < MAP_DIMENSION; i++)
		{
			for (int j = 0; j < MAP_DIMENSION; j++)
			{
				mCells[i][j].position = sf::Vector2f(mCells[i][j].position.x + MOVEMENT_FACTOR * dt, mCells[i][j].position.y);
			}
		}
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		for (int i = 0; i < MAP_DIMENSION; i++)
		{
			for (int j = 0; j < MAP_DIMENSION; j++)
			{
				mCells[i][j].position = sf::Vector2f(mCells[i][j].position.x, mCells[i][j].position.y - MOVEMENT_FACTOR * dt);
			}
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		for (int i = 0; i < MAP_DIMENSION; i++)
		{
			for (int j = 0; j < MAP_DIMENSION; j++)
			{
				mCells[i][j].position = sf::Vector2f(mCells[i][j].position.x, mCells[i][j].position.y + MOVEMENT_FACTOR * dt);
			}
		}
	}

	//ZOOM FACTOR
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
	{
		CELL_SIZE = CELL_SIZE + ZOOM_FACTOR * dt;

		for (int i = 0; i < MAP_DIMENSION; i++)
		{
			for (int j = 0; j < MAP_DIMENSION; j++)
			{
				mCells[i][j].position = sf::Vector2f(mCells[i][j].position.x + i * ZOOM_FACTOR * dt, mCells[i][j].position.y + j * ZOOM_FACTOR * dt);
			}
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
	{
		CELL_SIZE = CELL_SIZE - ZOOM_FACTOR * dt;

		for (int i = 0; i < MAP_DIMENSION; i++)
		{
			for (int j = 0; j < MAP_DIMENSION; j++)
			{
				mCells[i][j].position = sf::Vector2f(mCells[i][j].position.x - i * ZOOM_FACTOR * dt, mCells[i][j].position.y - j * ZOOM_FACTOR * dt);
			}
		}
	}

}

void Map::ProcessInput(sf::RenderWindow & window)
{
	sf::Vector2i windowPos = sf::Mouse::getPosition(window);

	// Mouse input
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		float xValue = std::floor((float)(windowPos.x) / CELL_SIZE);
		float yValue = std::floor((float)(windowPos.y) / CELL_SIZE);

		bool found = false;
		int xIndex = 0;
		int yIndex = 0;

		for (int i = 0; i < MAP_DIMENSION && !found; i++)
		{
			if (mCells[i][0].position.x < windowPos.x && mCells[i][0].position.x + CELL_SIZE > windowPos.x)
			{
				found = true;
				xIndex = i;
			}
		}

		found = false;

		for (int j = 0; j < MAP_DIMENSION && !found; j++)
		{
			if (mCells[0][j].position.y < windowPos.y && mCells[0][j].position.y + CELL_SIZE > windowPos.y)
			{
				found = true;
				yIndex = j;
			}
		}

		std::cout << "Cell: " << xIndex << "x" << yIndex << std::endl;
	}
}

void Map::Draw(sf::RenderWindow& window, sf::RenderStates states)
{
	float ratio = CELL_SIZE / std::max(OLD_CELL_SIZE, 0.001f);
	mSprite.setScale(ratio / 3, ratio / 3);

	for (int i = 0; i < MAP_DIMENSION; i++)
	{
		for (int j = 0; j < MAP_DIMENSION; j++)
		{
			Cell cell = mCells[i][j];

			mSprite.setPosition(cell.position);

			if (cell.type == eGround)
			{
				mSprite.setTexture(mGroundTexture);
			}
			else if (cell.type == eObstacle)
			{
				mSprite.setTexture(mObstacleTexture);
			}
			else if (cell.type == eStart)
			{
				mSprite.setTexture(mStartTexture);
			}
			else if (cell.type == eEnd)
			{
				mSprite.setTexture(mEndTexture);
			}
			else
			{
				std::cout << "Cell type not found when calling draw function" << std::endl;
			}

			//if (mColorBool)
			//	mSprite.setColor(sf::Color::Blue);
			//else
			//	mSprite.setColor(sf::Color::Red);

			//mColorBool = !mColorBool;

			window.draw(mSprite, states);
		}
	}
	OLD_CELL_SIZE = CELL_SIZE;
}
