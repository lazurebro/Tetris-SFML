#include <fstream>
#include "SFML\Graphics.hpp"
#include "Globals.h"

#include"Shapes.h"
#include"Tetris.h"


int main() {
	sf::RenderWindow window(sf::VideoMode(COLUMNS * CELL_SIZE * 2, ROWS * CELL_SIZE), "Tetris", sf::Style::Close);

	Tetris tetris(&window);
	sf::Clock deltaClock;

	tetris.start();

	while (window.isOpen())
	{
		sf::Time deltaTime = deltaClock.restart();

		float fps = 1.0f / deltaTime.asSeconds();
		window.setFramerateLimit(60);

		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case (sf::Event::Closed):
				window.close();
				break;
			default:
				tetris.handleEvent(event);
				break;
			}
		}

		tetris.update(deltaTime.asSeconds());

		window.clear();
		tetris.draw();
		window.display();
	}

	return 0;
}