#pragma once

#include "Engine/UI/Panel/UIPanel.h"
#include "Engine/Entities/CharacterStats.h"
#include "Engine/Core/Style.h"

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
class InfoPanel : public UIPanel
{
public:
	InfoPanel(float x, float y, float width, float height);

	void Update(float deltaTime) override;
	void Render(sf::RenderWindow& window, const sf::Font& font) override;

	// 데이터 소스 연결 (pull)
	void SetSource(const CharacterStats* stats);

	// 상태효과 관리
	void AddStatusEffect(const std::string& name, sf::Color color, int duration = -1);
	void RemoveStatusEffect(const std::string& name);
	void ClearStatusEffects();

private:
	const CharacterStats* StatsPtr = nullptr; // 데이터 소스

	std::vector<StatusEffect> StatusEffects; // 상태 효과

	// 프로그래스바 랜더링
	void DrawProgressBar(sf::RenderWindow& window, const sf::Font& font, float x, float y, float width, float height,
		int current, int max, sf::Color barColor, sf::Color bgColor);
};
