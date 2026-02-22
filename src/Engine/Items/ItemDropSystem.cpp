#include "Engine/Items/ItemDropSystem.h"

#include <algorithm>

#include "Engine/World/Map.h"
#include "Engine/Core/Macros.h"

ItemDropSystem::ItemDropSystem()
	: DefaultItemsPool(BuildDefaultItemPool())
{
}

void ItemDropSystem::Reset()
{
	GroundItems.clear();
}

void ItemDropSystem::SpawnOnLevel(const Map& map, const sf::Vector2i& playerPos, int spawnCount)
{
	// 스폰 가능한 후보 타일 수집
	std::vector<sf::Vector2i> candidates;
	candidates.reserve(static_cast<size_t>(map.GetWidth() * map.GetHeight()));

	for (int y = 1; y < map.GetHeight() - 1; ++y)
	{
		for (int x = 1; x < map.GetWidth() - 1; ++x)
		{
			if (!map.GetTile(x, y).Walkable)
			{
				continue;
			}

			if (x == playerPos.x && y == playerPos.y)
			{
				continue;
			}

			if (HasItemAt(x, y))
			{
				continue;
			}

			candidates.push_back({ x, y });
		}
	}

	if (candidates.empty())
	{
		return;
	}

	static std::mt19937 rng(std::random_device{}());
	std::shuffle(candidates.begin(), candidates.end(), rng);

	// 요청 개수와 후보 개수를 비교해 실제 스폰 개수를 계산
	int clampedSpawnCount = std::min(static_cast<int>(candidates.size()), std::max(1, spawnCount));
	for (int i = 0; i < clampedSpawnCount; ++i)
	{
		GroundItemEntry item;
		item.Position = candidates[static_cast<size_t>(i)];
		item.Archetype = PickRandomArchetype(rng);
		GroundItems.push_back(item);
		GAME_DEBUG_LOG("SpawnOnLevel", "아이템 스폰: " + item.Archetype.Name);
	}
}

bool ItemDropSystem::HasItemAt(int x, int y) const
{
	for (const GroundItemEntry& item : GroundItems)
	{
		if (item.Position.x == x && item.Position.y == y)
		{
			return true;
		}
	}
	return false;
}

ItemArchetype ItemDropSystem::PickRandomArchetype(std::mt19937& rng) const
{
	if (DefaultItemsPool.empty())
	{
		return {};
	}

	std::uniform_int_distribution<size_t> dist(0, DefaultItemsPool.size() - 1);
	return DefaultItemsPool[dist(rng)];
}
