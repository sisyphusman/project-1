#include "DungeonManager.h"

DungeonManager::DungeonManager(int mapWidth, int mapHeight)
	: MapWidth(mapWidth)
	, MapHeight(mapHeight)
{
	GenerateLevel(0);
}

void DungeonManager::GenerateLevel(int level)
{
	// 새 맵 생성
	auto map = std::make_unique<Map>(MapWidth, MapHeight);

	// 던전 구조 생성 (방 10개, 크기 5~12)
	DungeonGenerator generator;
	generator.Generate(*map, 10, 5, 12);

	const auto& rooms = generator.GetRooms();

	// 계단 위치 초기화 (-1은 계단 없음)
	sf::Vector2i upStairs(-1, -1);
	sf::Vector2i downStairs(-1, -1);

	if (rooms.size() >= 2)
	{
		// 첫 번째 방 중앙에 위층 계단
		if (level > 0)
		{
			upStairs = { rooms[0].CenterX(), rooms[0].CenterY() };
			map->SetTile(upStairs.x, upStairs.y, Tile::StairUp());
		}

		// 마지막 방 중앙에 아래층 계단
		downStairs = { rooms.back().CenterX(), rooms.back().CenterY() };
		map->SetTile(downStairs.x, downStairs.y, Tile::StairDown());
	}

	// 생성된 맵과 계단 위치를 저장
	Levels.push_back(std::move(map));
	StairsPositions.push_back({ upStairs, downStairs });
}

bool DungeonManager::GoToNextLevel(sf::Vector2i& outPlayerPos)
{
	int nextLevel = CurrentLevel + 1;

	// 새 레벨이 필요하면 생성
	if (nextLevel >= static_cast<int>(Levels.size()))
	{
		GenerateLevel(nextLevel);
	}

	CurrentLevel = nextLevel;

	// 위층 계단 위치에서 시작
	outPlayerPos = StairsPositions[CurrentLevel].first;

	// 항상 true (아래로는 무한히 이동 가능)
	return true;
}

bool DungeonManager::GoToPrevLevel(sf::Vector2i& outPlayerPos)
{
	// 0층에서는 위로 올라갈 수 없음
	if (CurrentLevel <= 0)
	{
		return false;
	}

	CurrentLevel--;

	// 아래층 계단 위치에서 시작
	outPlayerPos = StairsPositions[CurrentLevel].second;

	return true;
}

Map& DungeonManager::GetCurrentMap()
{
	return *Levels[CurrentLevel];
}
