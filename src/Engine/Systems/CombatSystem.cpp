#include "CombatSystem.h"

#include <algorithm>
#include <cstdlib>

#include "Engine/World/Map.h"

void CombatSystem::Reset()
{
	Monsters.clear();
}

bool CombatSystem::SpawnTestMonster(const Map& map, const sf::Vector2i& playerPos)
{
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

			CombatMonster monster;
			monster.Position = { x, y };
			monster.Glyph = 'g';
			monster.Stats.Level = 1;
			monster.Stats.HP = { 24, 24 };
			monster.Stats.STR = 9;
			monster.Stats.Defense = 2;
			monster.Stats.Attack = 7;
			Monsters.push_back(monster);
			return true;
		}
	}

	return false;
}

bool CombatSystem::HandlePlayerTurn(const Map& map, const sf::Vector2i& playerPos, int dx, int dy, CharacterStats& playerStats, bool& bOutPlayerMoved, 
	sf::Vector2i& outPlayerPos, std::vector<std::string>& outMessages, std::vector<CombatDamageEvent>& outDamageEvents)
{

	bOutPlayerMoved = false;
	outPlayerPos = playerPos;

	int targetX = playerPos.x + dx;
	int targetY = playerPos.y + dy;

	int monsterIndex = FindMonsterAt(targetX, targetY);
	if (monsterIndex >= 0)
	{
		CombatMonster& targetMonster = Monsters[static_cast<size_t>(monsterIndex)];
		int			   damage = CalculatePhysicalDamage(playerStats, targetMonster.Stats);
		targetMonster.Stats.HP.Modify(-damage);
		outDamageEvents.push_back({ targetMonster.Position, damage, true });
		outMessages.push_back("플레이어가 고블린에게 " + std::to_string(damage) + "의 물리 피해");

		if (targetMonster.Stats.HP.Current <= 0)
		{
			targetMonster.bIsAlive = false;
			outMessages.push_back("고블린을 처치했습니다");
		}
	}
	else
	{
		if (!map.IsWalkable(targetX, targetY))
		{
			return false;
		}

		outPlayerPos = { targetX, targetY };
		bOutPlayerMoved = true;
		outMessages.push_back("이동했습니다");
	}

	ProcessMonsterTurn(outPlayerPos, playerStats, outMessages, outDamageEvents);
	return true;
}

int CombatSystem::FindMonsterAt(int x, int y) const
{
	for (size_t i = 0; i < Monsters.size(); ++i)
	{
		const CombatMonster& monster = Monsters[i];
		if (monster.bIsAlive && monster.Position.x == x && monster.Position.y == y)
		{
			return static_cast<int>(i);
		}
	}

	return -1;
}

int CombatSystem::CalculatePhysicalDamage(const CharacterStats& attacker, const CharacterStats& defender) const
{
	int baseAttack = attacker.Attack + attacker.STR / 2;
	int mitigation = defender.Defense;
	return std::max(1, baseAttack - mitigation);
}

void CombatSystem::ProcessMonsterTurn(const sf::Vector2i& playerPos, CharacterStats& playerStats, std::vector<std::string>& outMessages, 
	std::vector<CombatDamageEvent>& outDamageEvents)
{
	for (CombatMonster& monster : Monsters)
	{
		if (!monster.bIsAlive)
		{
			continue;
		}

		int dx = std::abs(monster.Position.x - playerPos.x);
		int dy = std::abs(monster.Position.y - playerPos.y);
		if (dx + dy == 1)
		{
			int damage = CalculatePhysicalDamage(monster.Stats, playerStats);
			playerStats.HP.Modify(-damage);
			outDamageEvents.push_back({ playerPos, damage, false });
			outMessages.push_back("고블린이 플레이어에게 " + std::to_string(damage) + "의 물리 피해");

			if (playerStats.HP.Current <= 0)
			{
				outMessages.push_back("플레이어가 쓰러졌습니다");
			}
		}
	}

	Monsters.erase(std::remove_if(Monsters.begin(), Monsters.end(), [](const CombatMonster& monster) { return !monster.bIsAlive; }), Monsters.end());
}
