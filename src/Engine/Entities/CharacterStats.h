#pragma once

#include <cstdint>

// 기본 자원 (HP, MP)
struct ResourceStat
{
	int Current = 0;
	int Max = 0;

	float GetRatio() const
	{
		return (Max > 0) ? static_cast<float>(Current) / Max : 0.f;
	}

	void SetFull() { Current = Max; }
	void Modify(int amount) { Current = std::clamp(Current + amount, 0, Max); }
};

// 캐릭터 스탯
struct CharacterStats
{
	// 레벨
	int Level = 1;
	int Exp = 0;
	int ExpToNext = 100;

	// 자원
	ResourceStat HP = { 100, 100 };
	ResourceStat MP = { 20, 20 };

	// 기본 스탯
	int STR = 10;
	int DEX = 10;
	int INT = 10;

	int Attack = 10;
	int Defense = 5;
};