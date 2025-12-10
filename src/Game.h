#pragma once

#include <SFML/Graphics.hpp>

class Game
{
public:
	Game();
	~Game();

	void Run();

private:
	void ProcessEvents();
	void Render();

	sf::RenderWindow window_;
};

