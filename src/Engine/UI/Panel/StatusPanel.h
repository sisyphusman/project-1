#pragma once

#include "Engine/UI/Panel/UIPanel.h"
#include "Engine/Entities/CharacterStats.h"

#include <vector>
#include <string>

// 상태 효과 (버프/디버프)
struct StatusEffect
{
	std::string Name;
	sf::Color	Color;
	int			Duration;
};

// 캐릭터 스탯 패널
class StatusPanel : public UIPanel
{
public:
	StatusPanel(float x, float y, float width, float height);

	void Update(float deltaTime) override;
	void Render(sf::RenderWindow& window, const sf::Font& font) override;

	// 스탯 전체를 한 번에 설정
	void SetStats(const CharacterStats& stats);

	// 상태효과 관리
	void AddStatusEffect(const std::string& name, sf::Color color, int duration = -1);
	void RemoveStatusEffect(const std::string& name);
	void ClearStatusEffects();

private:
	const CharacterStats* StatsPtr = nullptr; // 포인터로 참조 (복사 방지)
	CharacterStats CachedStats;				  // 포인터 없을 때 사용

	// 상태 효과
	std::vector<StatusEffect> StatusEffects;

	// 프로그래스바 랜더링
	void DrawProgressBar(sf::RenderWindow& window, const sf::Font& font, float x, float y, float width, float height,
		int current, int max, sf::Color barColor, sf::Color bgColor, const std::string& label);
};
