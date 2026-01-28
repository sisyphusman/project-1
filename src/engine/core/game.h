#pragma once

#include <SFML/Graphics.hpp>
#include "engine/manager/input_manager.h"

class Game
{
public:
	Game();
	~Game();

	void Run();

private:
	void ProcessEvents();
	void Update();
	void Render();

	sf::RenderWindow window_;
	InputManager input_manager_;
};

