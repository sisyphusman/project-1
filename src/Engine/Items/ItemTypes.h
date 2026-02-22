#pragma once

#include <string>
#include <vector>

#include <SFML/System/Vector2.hpp>

// 아이템 타입
enum class ItemKind
{
	Consumable,
	Equipment,
	Misc
};

// 소비 아이템의 효과 타입
enum class ConsumableEffectType
{
	None,
	HealHP,
	RestoreMP
};

// 아이템 원형 데이터
struct ItemArchetype
{
	std::string			 Id;
	std::string			 Name;
	char				 Glyph = '?';
	ItemKind			 Kind = ItemKind::Consumable;
	ConsumableEffectType EffectType = ConsumableEffectType::None;
	int					 EffectValue = 0;
	bool				 Stackable = true;
};

// 바닥 아이템 인스턴스
struct GroundItemEntry
{
	ItemArchetype Archetype;
	sf::Vector2i  Position;
};

inline std::vector<ItemArchetype> BuildDefaultItemPool()
{
	return {
		{ "hp_small", "작은 치유 물약", '?', ItemKind::Consumable, ConsumableEffectType::HealHP, 20, true },
		{ "hp_large", "큰 치유 물약", '?', ItemKind::Consumable, ConsumableEffectType::HealHP, 40, true },
		{ "mp_small", "작은 마나 물약", '?', ItemKind::Consumable, ConsumableEffectType::RestoreMP, 10, true },
	};
}