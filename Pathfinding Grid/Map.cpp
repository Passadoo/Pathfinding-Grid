#include "Map.h"



Map::Map()
{

	mGroundTexture.loadFromFile("Assets/GroundTexture.png");
	mObstacleTexture.loadFromFile("Assets/ObstacleTexture.png");
	mStartTexture.loadFromFile("Assets/StartTexture.png");
	mEndTexture.loadFromFile("Assets/EndTexture.png");

	//mSprite.scale(0.25f, 0.25f);

	//Create the 2 dimensional array
	mGrid.cells = new Cell*[mGrid.size];

	for (int i = 0; i < mGrid.size; i++)
	{
		mGrid.cells[i] = new Cell[mGrid.size];
	}

	//Fill the array with ground tiles
	for (int i = 0; i < mGrid.size; i++)
	{
		for (int j = 0; j < mGrid.size; j++)
		{
			mGrid.cells[i][j].xindex = i;
			mGrid.cells[i][j].yindex = j;
			mGrid.cells[i][j].position = sf::Vector2f(mGrid.cellSize * i, mGrid.cellSize * j);
			mGrid.cells[i][j].type = eGround;
		}
	}

	std::cout << "Use Arrow keys to move" << std::endl;
	std::cout << "Use the K and L keys to Zoom" << std::endl;
	std::cout << "Useing normal A* pathfinding" << std::endl;
}


Map::~Map()
{
	mGrid.cleanUp();
}

void Map::update(float dt)
{
	//MOVE FACTOR
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		for (int i = 0; i < mGrid.size; i++)
		{
			for (int j = 0; j < mGrid.size; j++)
			{
				mGrid.cells[i][j].position = sf::Vector2f(mGrid.cells[i][j].position.x - MOVEMENT_FACTOR * dt, mGrid.cells[i][j].position.y);
			}
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		for (int i = 0; i < mGrid.size; i++)
		{
			for (int j = 0; j < mGrid.size; j++)
			{
				mGrid.cells[i][j].position = sf::Vector2f(mGrid.cells[i][j].position.x + MOVEMENT_FACTOR * dt, mGrid.cells[i][j].position.y);
			}
		}
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		for (int i = 0; i < mGrid.size; i++)
		{
			for (int j = 0; j < mGrid.size; j++)
			{
				mGrid.cells[i][j].position = sf::Vector2f(mGrid.cells[i][j].position.x, mGrid.cells[i][j].position.y - MOVEMENT_FACTOR * dt);
			}
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		for (int i = 0; i < mGrid.size; i++)
		{
			for (int j = 0; j < mGrid.size; j++)
			{
				mGrid.cells[i][j].position = sf::Vector2f(mGrid.cells[i][j].position.x, mGrid.cells[i][j].position.y + MOVEMENT_FACTOR * dt);
			}
		}
	}

	//ZOOM FACTOR
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
	{
		mGrid.cellSize = mGrid.cellSize + ZOOM_FACTOR * dt;

		for (int i = 0; i < mGrid.size; i++)
		{
			for (int j = 0; j < mGrid.size; j++)
			{
				mGrid.cells[i][j].position = sf::Vector2f(mGrid.cells[i][j].position.x + i * ZOOM_FACTOR * dt, mGrid.cells[i][j].position.y + j * ZOOM_FACTOR * dt);
			}
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
	{
		mGrid.cellSize = mGrid.cellSize - ZOOM_FACTOR * dt;

		for (int i = 0; i < mGrid.size; i++)
		{
			for (int j = 0; j < mGrid.size; j++)
			{
				mGrid.cells[i][j].position = sf::Vector2f(mGrid.cells[i][j].position.x - i * ZOOM_FACTOR * dt, mGrid.cells[i][j].position.y - j * ZOOM_FACTOR * dt);
			}
		}
	}

}

void Map::ProcessInput(sf::RenderWindow & window)
{
	sf::Vector2i windowPos = sf::Mouse::getPosition(window);

	// Mouse input
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !mlDown)
	{
		mlDown = true;
		float xValue = std::floor((float)(windowPos.x) / mGrid.cellSize);
		float yValue = std::floor((float)(windowPos.y) / mGrid.cellSize);

		bool found = false;
		int xIndex = 0;
		int yIndex = 0;

		for (int i = 0; i < mGrid.size && !found; i++)
		{
			if (mGrid.cells[i][0].position.x < windowPos.x && mGrid.cells[i][0].position.x + mGrid.cellSize > windowPos.x)
			{
				found = true;
				xIndex = i;
			}
		}

		found = false;

		for (int j = 0; j < mGrid.size && !found; j++)
		{
			if (mGrid.cells[0][j].position.y < windowPos.y && mGrid.cells[0][j].position.y + mGrid.cellSize > windowPos.y)
			{
				found = true;
				yIndex = j;
			}
		}

		std::cout << "Cell: " << xIndex << "x" << yIndex << std::endl;

		switch (hasStartEnd)
		{
		case 0:
			start.x = xIndex;
			start.y = yIndex;
			hasStartEnd++;
			if (startCellOld)
				startCellOld->type = CellType::eGround;
			mGrid.cells[xIndex][yIndex].type = CellType::eStart;
			startCellOld = &mGrid.cells[xIndex][yIndex];
			std::cout << "set start" << std::endl;
			break;
		case 1:
			end.x = xIndex;
			end.y = yIndex;
			hasStartEnd = 0;
			if (endCellOld)
				endCellOld->type = CellType::eGround;
			mGrid.cells[xIndex][yIndex].type = CellType::eEnd;
			endCellOld = &mGrid.cells[xIndex][yIndex];
			std::cout << "set goal" << std::endl;
			break;
		default:
			break;
		}
	}
	
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		mlDown = false;
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !mrDown)
	{
		mrDown = true;
		std::cout << "compute path" << std::endl;
		for (int i = 0; i < mGrid.size; i++)
		{
			for (int j = 0; j < mGrid.size; j++)
			{
				mGrid.cells[i][j].type = CellType::eGround;
			}
		}
		path.clear();
		if (DirBasedPathFinding)
			path = PathFinding::computeDirectionBasedAStart(start, end, mGrid);
		else
			path = PathFinding::computeAStart(start, end, mGrid);
	}
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		mrDown = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) && !XDown)
	{
		XDown = true;
		DirBasedPathFinding = !DirBasedPathFinding;

		if (DirBasedPathFinding)
			std::cout << "Useing direction based A* pathfinding" << std::endl;
		else
			std::cout << "Useing normal A* pathfinding" << std::endl;
	}
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::X))
	{
		XDown = false;
	}
}

void Map::Draw(sf::RenderWindow& window, sf::RenderStates states)
{
	float ratio = mGrid.cellSize / START_SIZE_OF_CELLS;
	mSprite.setScale(ratio / 3, ratio / 3);

	for (int i = 0; i < mGrid.size; i++)
	{
		for (int j = 0; j < mGrid.size; j++)
		{
			Cell cell = mGrid.cells[i][j];
			mSprite.setColor(sf::Color::White);
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
			else if (cell.type == ePath)
			{
				mSprite.setTexture(mGroundTexture);
				mSprite.setColor(sf::Color::Blue);
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
}
