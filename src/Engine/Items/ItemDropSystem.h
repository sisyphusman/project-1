#pragma once

#include <random>
#include <vector>

#include "Engine/Items/ItemTypes.h"

class Map;

// 층별 바닥 아이템 관리 시스템
class ItemDropSystem
{
public:
	ItemDropSystem();

	void Reset();

	void SpawnOnLevel(const Map& map, const sf::Vector2i& playerPos, int spawnCount = 4); // 층 진입 시 아이템들 흩뿌려 배치

	bool HasItemAt(int x, int y) const; // 현재 타일에 아이템이 존재하는지 확인

	const std::vector<GroundItemEntry>& GetGroundItems() const { return GroundItems; }

private:
	ItemArchetype PickRandomArchetype(std::mt19937& rng) const; // 기본 아이템 풀에서 한 개를 랜덤 선택

	std::vector<GroundItemEntry> GroundItems;	   // 현재 층에 배치된 바닥 아이템 목록
	std::vector<ItemArchetype>	 DefaultItemsPool; // 기본 아이템 드랍 테이블
};