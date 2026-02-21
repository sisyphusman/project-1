#include "TurnSystem.h"

#include "Engine/Systems/FOV.h"
#include "Engine/World/Map.h"

void TurnSystem::Reset()
{
	IsPlayerTurn = true;
	SeenEnemyIds.clear();
}

bool TurnSystem::ExecutePlayerTurn(CombatSystem& combat, const Map& map, const sf::Vector2i& playerPos, int dx, int dy, CharacterStats& playerStats, TurnResult& outResult)
{
	if (!IsPlayerTurn)
	{
		return false;
	}

	outResult = {};
	outResult.PlayerNextPosition = playerPos;

	if (!combat.HandlePlayerAction(map, playerPos, dx, dy, playerStats, outResult.PlayerMoved, outResult.PlayerNextPosition, outResult.Messages,
			outResult.DamageEvents, outResult.DefeatedEnemyCount))
	{
		return false;
	}

	// 플레이어 입력이 확정되면 에너미 턴으로 넘긴 뒤 다시 플레이어 턴으로 리턴
	IsPlayerTurn = false;
	combat.ProcessEnemyTurn(outResult.PlayerNextPosition, playerStats, outResult.Messages, outResult.DamageEvents);
	IsPlayerTurn = true;
	return true;
}

void TurnSystem::CollectNewVisibleEnemyMessages(const CombatSystem& combat, const FOV& fov, std::vector<std::string>& outMessages)
{
	for (const CombatEnemy& enemy : combat.GetEnemies())
	{
		if (!enemy.IsAlive)
		{
			continue;
		}

		if (!fov.IsVisible(enemy.Position.x, enemy.Position.y))
		{
			continue;
		}

		if (SeenEnemyIds.contains(enemy.Id))
		{
			continue;
		}

		SeenEnemyIds.insert(enemy.Id);
		outMessages.push_back(enemy.Name + "이 시야에 나타났습니다");
	}
}