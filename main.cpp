#include <SFML/Graphics.hpp>
#include "Headers/Global.hpp"
#include "Headers/MapManager.hpp"
#include "Headers/Student.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode(sf::Vector2u{static_cast<unsigned>(SCREEN_WIDTH * SCREEN_RESIZE), static_cast<unsigned>(SCREEN_HEIGHT * SCREEN_RESIZE)}), "Super mario game");
	window.setFramerateLimit(60);

	MapManager map_manager;
	Student student;
	student.set_position(100.0f, static_cast<float>(SCREEN_HEIGHT - CELL_SIZE - 64));

	unsigned view_x = 0;

	while (window.isOpen())
	{
		while (auto event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();
		}

		map_manager.update();
		student.update(view_x, map_manager);

		window.clear(sf::Color::Black);
		map_manager.draw_map(false, false, view_x, window);
		student.draw(window);
		window.display();
	}

	return 0;
}
