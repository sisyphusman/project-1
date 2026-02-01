#pragma once

enum class TileType
{
	Floor,
	Wall,
	StairDown,
	StairUp
};

// 정적 팩토리 메서드 패턴 적용
struct Tile
{
	TileType Type = TileType::Floor;
	char	 Glyph = '.';
	bool	 Walkable = true;
	bool	 Transparent = true; // 시야가 통과하는 타일

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