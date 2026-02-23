#pragma once

#include <string>
#include <unordered_set>
#include <vector>

#include <SFML/System/Vector2.hpp>

#include "Engine/Data/MessageCatalog.h"
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
	void Reset(); // 층 전환 시 봤던 Enemy 기록을 초기화

	// 플레이어 입력을 1턴으로 처리, 이후 Enemy 턴 진행, 결과를 outResult로 반환, false이면 턴이 소비되지 않은 잘못된 입력이다
	bool ExecuteTurn(CombatSystem& combat, const Map& map, const sf::Vector2i& playerPos, int dx, int dy, CharacterStats& playerStats,
		TurnResult& outResult);

	// 시야에 첫 등장하는 Enemy 등장 메시지를 수집
	void CollectNewVisibleEnemyMessages(const CombatSystem& combat, const FOV& fov, std::vector<std::string>& outMessages);

	void SetMessageCatalog(const MessageCatalog* catalog) { OutMessageCatalog = catalog; }

private:
	bool					IsPlayerTurn = true; // 턴 상태 플래그
	std::unordered_set<int> SeenEnemyIds;		 // 한번 본 Enemy 리스트

	const MessageCatalog* OutMessageCatalog = nullptr;
};