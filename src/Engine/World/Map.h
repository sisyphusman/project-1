#pragma once

#include "Engine/World/Tile.h"
#include <vector>
#include <SFML/System/Vector2.hpp>

class Map
{
public:
	Map(int width, int height);

	const Tile& GetTile(int x, int y) const;			 // 타일 조회용
	void		SetTile(int x, int y, const Tile& tile); // 타일 변경용
	bool		IsWalkable(int x, int y) const;
	bool		InBounds(int x, int y) const;

	int GetWidth() const { return Width; }
	int GetHeight() const { return Height; }

private:
	int				  Width;
	int				  Height;
	std::vector<Tile> Tiles;
};