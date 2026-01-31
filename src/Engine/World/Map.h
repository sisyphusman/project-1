#pragma once

#include "Engine/World/Tile.h"
#include <vector>
#include <SFML/System/Vector2.hpp>

class Map
{
public:
	Map(int width, int height);

	Tile&		GetTile(int x, int y);		 // 타일 변경용
	const Tile& GetTile(int x, int y) const; // 타일 조회용
	bool		IsWalkable(int x, int y) const;
	bool		InBounds(int x, int y) const;

	int GetWidth() const { return Width; }
	int GetHeight() const { return Height; }

private:
	int				  Width;
	int				  Height;
	std::vector<Tile> Tiles;
};