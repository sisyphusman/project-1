#include "Engine/Systems/CombatSystem.h"

#include <algorithm>
#include <random>

#include "Engine/World/Map.h"

void CombatSystem::Reset()
{
	Enemies.clear();
	NextEnemyId = 1;
}

void CombatSystem::SaveState(std::vector<CombatEnemy>& outEnemies, int& outNextEnemyId) const
{
	outEnemies = Enemies;
	outNextEnemyId = NextEnemyId;
}

void CombatSystem::LoadState(const std::vector<CombatEnemy>& enemies, int nextEnemyId)
{
	Enemies = enemies;
	NextEnemyId = std::max(1, nextEnemyId);
}

bool CombatSystem::SpawnTestEnemy(const Map& map, const sf::Vector2i& playerPos)
{
	// 후보 지역을 모아서 랜덤으로 픽
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

			if (FindEnemyAt(x, y) >= 0)
			{
				continue;
			}

			candidates.push_back({ x, y });
		}
	}

	if (candidates.empty())
	{
		return false;
	}

	static std::mt19937					  rng(std::random_device{}());
	std::uniform_int_distribution<size_t> dist(0, candidates.size() - 1);
	sf::Vector2i						  spawnPos = candidates[dist(rng)];

	const EnemyTemplate* selectedTemplate = nullptr;
	if (EnemyDataCatalog != nullptr)
	{
		// 랜덤한 에너미 카탈로그를 로드
		selectedTemplate = EnemyDataCatalog->PickRandomTemplate(rng);
	}

	CombatEnemy enemy;
	enemy.Id = NextEnemyId++;
	enemy.Position = spawnPos;

	if (selectedTemplate != nullptr)
	{
		// JSON 템플릿 기반
		enemy.TemplateId = selectedTemplate->Id;
		enemy.Name = selectedTemplate->Name;
		enemy.Glyph = selectedTemplate->Glyph;
		enemy.Stats = selectedTemplate->BaseStats;
		enemy.SpritePath = selectedTemplate->SpritePath;
	}
	else
	{
		// 카탈로그 로딩 실패 시 기본값
		enemy.TemplateId = "fallback_ghost";
		enemy.Name = "귀신";
		enemy.Glyph = 'g';
		enemy.Stats.Level = 1;
		enemy.Stats.HP = { 24, 24 };
		enemy.Stats.STR = 9;
		enemy.Stats.Defense = 2;
		enemy.Stats.Attack = 7;
		enemy.SpritePath = "assets/Ghost.png";
	}

	Enemies.push_back(enemy);
	return true;
}

bool CombatSystem::HandlePlayerAction(const Map& map, const sf::Vector2i& playerPos, int dx, int dy, CharacterStats& playerStats, bool& bOutPlayerMoved,
	sf::Vector2i& outPlayerPos, std::vector<std::string>& outMessages, std::vector<CombatDamageEvent>& outDamageEvents, int& outDefeatedEnemyCount)
{

	bOutPlayerMoved = false;
	outPlayerPos = playerPos;
	outDefeatedEnemyCount = 0;

	int targetX = playerPos.x + dx;
	int targetY = playerPos.y + dy;

	int enemyIndex = FindEnemyAt(targetX, targetY);
	if (enemyIndex >= 0)
	{
		CombatEnemy& targetEnemy = Enemies[static_cast<size_t>(enemyIndex)];
		int			 damage = CalculatePhysicalDamage(playerStats, targetEnemy.Stats);
		targetEnemy.Stats.HP.Modify(-damage);
		outDamageEvents.push_back({ targetEnemy.Position, damage, true });
		outMessages.push_back("플레이어가 " + targetEnemy.Name + "에게 " + std::to_string(damage) + "의 물리 피해");

		if (targetEnemy.Stats.HP.Current <= 0)
		{
			targetEnemy.IsAlive = false;
			outDefeatedEnemyCount++;
			outMessages.push_back(targetEnemy.Name + "을 처치했습니다");
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
	}

	RemoveDeadEnemies();
	return true;
}

int CombatSystem::FindEnemyAt(int x, int y) const
{
	for (size_t i = 0; i < Enemies.size(); ++i)
	{
		const CombatEnemy& enemy = Enemies[i];
		if (enemy.IsAlive && enemy.Position.x == x && enemy.Position.y == y)
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

void CombatSystem::RemoveDeadEnemies()
{
	Enemies.erase(std::remove_if(Enemies.begin(), Enemies.end(), [](const CombatEnemy& enemy) { return !enemy.IsAlive; }), Enemies.end());
}

void CombatSystem::ProcessEnemyTurn(const sf::Vector2i& playerPos, CharacterStats& playerStats, std::vector<std::string>& outMessages,
	std::vector<CombatDamageEvent>& outDamageEvents)
{
	for (CombatEnemy& enemy : Enemies)
	{
		if (!enemy.IsAlive)
		{
			continue;
		}

		int dx = std::abs(enemy.Position.x - playerPos.x);
		int dy = std::abs(enemy.Position.y - playerPos.y);
		if (dx + dy == 1)
		{
			int damage = CalculatePhysicalDamage(enemy.Stats, playerStats);
			playerStats.HP.Modify(-damage);
			outDamageEvents.push_back({ playerPos, damage, false });
			outMessages.push_back(enemy.Name + "이 플레이어에게 " + std::to_string(damage) + "의 물리 피해");

			if (playerStats.HP.Current <= 0)
			{
				outMessages.push_back("플레이어가 쓰러졌습니다");
			}
		}
	}

	Enemies.erase(std::remove_if(Enemies.begin(), Enemies.end(), [](const CombatEnemy& enemy) { return !enemy.IsAlive; }), Enemies.end());
}
