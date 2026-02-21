#pragma once

#include <string>
#include <vector>

#include <SFML/System/Vector2.hpp>

#include "Engine/Data/EnemyCatalog.h"
#include "Engine/Entities/CharacterStats.h"

class Map;

struct CombatEnemy
{
	int			   Id = 0;
	std::string	   TemplateId;
	std::string	   Name = "귀신";
	std::string	   SpritePath;
	sf::Vector2i   Position;
	char		   Glyph = 'g';
	CharacterStats Stats;
	bool		   IsAlive = true;
};

struct CombatDamageEvent
{
	sf::Vector2i TilePosition;
	int			 Damage = 0;
	bool		 FromPlayer = false;
};

class CombatSystem
{
public:
	void Reset();
	void SaveState(std::vector<CombatEnemy>& outEnemies, int& outNextEnemyId) const; // const는 이 객체의 멤버 변수 값은 그대로임을 보장
	void LoadState(const std::vector<CombatEnemy>& enemies, int nextEnemyId);		 // 매개 변수 enemies만 그대로임을 보장
	void SetEnemyCatalog(const EnemyCatalog* catalog) { EnemyDataCatalog = catalog; }
	bool SpawnTestEnemy(const Map& map, const sf::Vector2i& playerPos); // 무작위로 몬스터 배치

	// 플레이어(이동/공격) 처리
	bool HandlePlayerAction(const Map& map, const sf::Vector2i& playerPos, int dx, int dy, CharacterStats& playerStats, bool& bOutPlayerMoved,
		sf::Vector2i& outPlayerPos, std::vector<std::string>& outMessages, std::vector<CombatDamageEvent>& outDamageEvents, int& outDefeatedEnemyCount);

	// 에너미 행동 처리
	void ProcessEnemyTurn(const sf::Vector2i& playerPos, CharacterStats& playerStats, std::vector<std::string>& outMessages,
		std::vector<CombatDamageEvent>& outDamageEvents);

	const std::vector<CombatEnemy>& GetEnemies() const { return Enemies; }

private:
	int	 FindEnemyAt(int x, int y) const;
	int	 CalculatePhysicalDamage(const CharacterStats& attacker, const CharacterStats& defender) const; // 공격력 + 힘 보정 - 방어력, 최소 1 보장
	void RemoveDeadEnemies();

	int						 NextEnemyId = 1; // 에너미 식별자
	std::vector<CombatEnemy> Enemies;		  // 현재 층 에너미 리스트

	const EnemyCatalog* EnemyDataCatalog = nullptr;
};