#include <SFML/Graphics.hpp>
#include "Map.h"
#include <ctime>

int main()
{
	std::srand(static_cast<int>(time(0)));

	Map map;
	sf::Clock clock;
	sf::RenderWindow window(sf::VideoMode(1080, 1080), "Pathfinding Grid");
	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();
		}
		
		if (clock.getElapsedTime().asSeconds() >= 0.01)
		{
			map.update(clock.getElapsedTime().asSeconds());
			map.ProcessInput(window);
			clock.restart();
		}

		window.clear();
		map.Draw(window);
		window.display();
	}

	return 0;
}