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

	sf::RenderWindow Window;
	sf::Clock		 FrameClock;

	// 플레이어
	sf::Font				PlayerFont;
	std::optional<sf::Text> PlayerText;
	sf::Vector2f			PlayerPos;
	float					PlayerSpeed;
};
