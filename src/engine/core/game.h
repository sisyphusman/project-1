#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

class Game
{
public:
	Game();
	~Game();

	void Run();

private:
	void Init();
	void ProcessEvents();
	void Update(float deltaTime);
	void Render();
	void Clear();

	sf::RenderWindow window_;
	sf::Clock clock_;
};

