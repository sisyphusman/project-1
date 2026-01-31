#include "Engine/World/Map.h"

Map::Map(int width, int height)
	: Width(width)
	, Height(height)
	, Tiles(width * height, Tile::Wall())
{
	// 초기화만 담당
	for (int y = 0; y < Height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			bool isEdge = (x == 0 || x == Width - 1 || y == 0 || y == Height - 1);
			Tiles[y * Width + x] = isEdge ? Tile::Wall() : Tile::Floor();
		}
	}
}

bool Map::InBounds(int x, int y) const
{
	return x >= 0 && x < Width && y >= 0 && y < Height;
}

void Map::SetTile(int x, int y, const Tile& tile)
{
	if (InBounds(x, y))
	{
		Tiles[y * Width + x] = tile;
	}
}

const Tile& Map::GetTile(int x, int y) const
{
	return Tiles[y * Width + x];
}

bool Map::IsWalkable(int x, int y) const
{
	return InBounds(x, y) && Tiles[y * Width + x].Walkable;
}