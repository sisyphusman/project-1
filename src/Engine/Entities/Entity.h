#pragma once
#include <SFML/System/Vector2.hpp>

class Map;

class Entity
{
public:
	Entity(int x, int y, char glyph);
	virtual ~Entity() = default;

	// 타일 기반 이동
	bool TryMove(int dx, int dy, const Map& map);
	void SetPosition(int x, int y);

	sf::Vector2i GetPosition() const { return Position; }
	char		 GetGlyph() const { return Glyph; }

private:
	sf::Vector2i Position;
	char		 Glyph;			// 나타낼 문자
};