#pragma once

enum class TileType
{
	Floor,
	Wall,
	StairDown,
	StairUp
};

struct Tile
{
	TileType Type = TileType::Floor;
	char	 Glyph = '.';
	bool	 Walkable = true;
	bool	 Transparent = true; // FOV 용도

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

	static Tile StairDown()
	{
		return {
			TileType::StairDown, '>', true, true
		};
	}

	static Tile StairUp()
	{
		return {
			TileType::StairUp, '<', true, true
		};
	}
};