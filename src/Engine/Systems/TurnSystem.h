#pragma once

#include <string>
#include <unordered_set>
#include <vector>

#include <SFML/System/Vector2.hpp>

#include "Engine/Systems/CombatSystem.h"

class Map;
class FOV;

struct TurnResult
{
	bool						   PlayerMoved = false;
	sf::Vector2i				   PlayerNextPosition;
	std::vector<std::string>	   Messages;
	std::vector<CombatDamageEvent> DamageEvents;
	int							   DefeatedEnemyCount = 0;
};

class TurnSystem
{
public:
	void Reset(); // 층 전환 시 에너미를 봤던 기록을 초기화

	// 플레이어 턴 -> 에너미 턴 순으로 진행
	bool ExecutePlayerTurn(CombatSystem& combat, const Map& map, const sf::Vector2i& playerPos, int dx, int dy, CharacterStats& playerStats,
		TurnResult& outResult);

	// 시야에 첫 등장하는 에너미의 등장 메시지를 수집
	void CollectNewVisibleEnemyMessages(const CombatSystem& combat, const FOV& fov, std::vector<std::string>& outMessages);

private:
	bool					IsPlayerTurn = true; // 턴 상태 플래그
	std::unordered_set<int> SeenEnemyIds;		  // 한번 본 에너미 리스트
};