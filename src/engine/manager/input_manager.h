#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <unordered_map>

class InputManager
{
public:
	InputManager();
	~InputManager();

	void Update();
	void SetKey(sf::Keyboard::Key key, bool pressed);

	bool IsKeyPressed(sf::Keyboard::Key key) const;

private:
	std::unordered_map<sf::Keyboard::Key, bool> prev_key_states_;
	std::unordered_map<sf::Keyboard::Key, bool> cur_key_states_;
};

