#pragma once

#include <string>
#include <vector>

#include <SFML/System/Vector2.hpp>

#include "Engine/Entities/CharacterStats.h"

class Map;

struct CombatMonster
{
	sf::Vector2i   Position;
	char		   Glyph = 'g';
	CharacterStats Stats;
	bool		   bIsAlive = true;
};

struct CombatDamageEvent
{
	sf::Vector2i TilePosition;
	int			 Damage = 0;
	bool		 bFromPlayer = false;
};

class CombatSystem
{
public:
	void Reset();
	bool SpawnTestMonster(const Map& map, const sf::Vector2i& playerPos);

	bool HandlePlayerTurn(const Map& map, const sf::Vector2i& playerPos, int dx, int dy, CharacterStats& playerStats, bool& bOutPlayerMoved,
		sf::Vector2i& outPlayerPos, std::vector<std::string>& outMessages, std::vector<CombatDamageEvent>& outDamageEvents);

	const std::vector<CombatMonster>& GetMonsters() const { return Monsters; }

private:
	int	 FindMonsterAt(int x, int y) const;
	int	 CalculatePhysicalDamage(const CharacterStats& attacker, const CharacterStats& defender) const;
	void ProcessMonsterTurn(const sf::Vector2i& playerPos, CharacterStats& playerStats, std::vector<std::string>& outMessages,
		std::vector<CombatDamageEvent>& outDamageEvents);

	std::vector<CombatMonster> Monsters;
};