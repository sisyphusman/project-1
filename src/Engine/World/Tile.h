#pragma once

enum class TileType
{
	Floor,
	Wall
};

struct Tile
{
	TileType Type = TileType::Floor;
	char	 Glyph = '.';
	bool	 Walkable = true;
	bool	 Transparent = true; // FOV 용도

	// 중괄호가 블록이 아니라 값이기 때문에 return과 같은 줄

	// 헬퍼 함수
	static Tile Floor()
	{
		return {
			TileType::Floor, '.', true, true
		};
	}

	static Tile Wall()
	{
		return {
			TileType::Wall, '#', false, false
		};
	}
};