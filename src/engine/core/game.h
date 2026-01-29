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

	// 플레이어
	sf::Font player_font_;
	std::optional<sf::Text> player_;
	sf::Vector2f player_pos_;
	float player_speed_ = 200.f;
};

