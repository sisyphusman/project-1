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

void ItemDropSystem::SetGroundItems(const std::vector<GroundItemEntry>& items)
{
	GroundItems = items;
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

	// 아이템 수가 타일 수보다 많을때 제한 처리
	const int safeSpawnCount = std::clamp(spawnCount, 0, static_cast<int>(candidates.size()));
	if (safeSpawnCount < spawnCount)
	{
		GAME_DEBUG_LOG("SpawnOnLevel", "땅에 떨어진 아이템이 타일 수보다 많아서 자동으로 축소됨");
	}

	// 아이템 요청 개수만큼 스폰
	for (int i = 0; i < safeSpawnCount; ++i)
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

std::vector<ItemArchetype> ItemDropSystem::GetItemAt(int x, int y) const
{
	std::vector<ItemArchetype> foundItems;

	for (const GroundItemEntry& item : GroundItems)
	{
		if (item.Position.x == x && item.Position.y == y)
		{
			foundItems.push_back(item.Archetype);
		}
	}

	return foundItems;
}

std::optional<ItemArchetype> ItemDropSystem::TryPickupAt(int x, int y)
{
	for (auto it = GroundItems.begin(); it != GroundItems.end(); ++it)
	{
		if (it->Position.x == x && it->Position.y == y)
		{
			ItemArchetype picked = it->Archetype;
			GroundItems.erase(it);
			return picked;
		}
	}

	return std::nullopt;
}

bool ItemDropSystem::TryDropAt(int x, int y, const ItemArchetype& item)
{
	GroundItemEntry entry;
	entry.Archetype = item;
	entry.Position = { x, y };
	GroundItems.push_back(entry);
	return true;
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
