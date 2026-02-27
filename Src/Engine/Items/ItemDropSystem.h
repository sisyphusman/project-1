#pragma once

#include <optional>
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

	void SetGroundItems(const std::vector<GroundItemEntry>& items); // 층의 저장된 아이템 복원

	void SpawnOnLevel(const Map& map, const sf::Vector2i& playerPos, int spawnCount = 4); // 층 진입 시 아이템들 흩뿌려 배치

	bool HasItemAt(int x, int y) const; // 현재 타일에 아이템이 존재하는지 확인

	// 타일 위 아이템을 제거하고 반환
	std::optional<ItemArchetype> TryPickupAt(int x, int y);

	// 아이템을 특정 타일에 떨어트림
	bool TryDropAt(int x, int y, const ItemArchetype& item);

	const std::vector<GroundItemEntry>& GetGroundItems() const { return GroundItems; }

private:
	ItemArchetype PickRandomArchetype(std::mt19937& rng) const; // 기본 아이템 풀에서 한 개를 랜덤 선택

	std::vector<GroundItemEntry> GroundItems;	   // 현재 층에 배치된 바닥 아이템 목록
	std::vector<ItemArchetype>	 DefaultItemsPool; // 기본 아이템 스폰 테이블
};