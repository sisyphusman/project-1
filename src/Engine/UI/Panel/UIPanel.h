#pragma once

#include <SFML/Graphics.hpp>

#include <Engine/Core/Style.h>

// UI 패널 기본 클래스
class UIPanel
{
public:
	UIPanel(float x, float y, float width, float height)
		: Bounds({ x, y }, { width, height })
	{
	}

	virtual ~UIPanel() = default;

	// 매 프레임 업데이트 (애니메이션 등)
	virtual void Update(float deltaTime) {}

	// 렌더링
	virtual void Render(sf::RenderWindow& window, const sf::Font& font) = 0;

	// 영역 접근
	sf::FloatRect GetBounds() const { return Bounds; }

protected:
	static constexpr Style DefaultStyle{}; // Style의 레이아웃 사용
	sf::FloatRect Bounds; // 패널 영역

	// 배경 그리기 헬퍼 함수
	void DrawBackground(sf::RenderWindow& window, sf::Color fillColor, sf::Color outlineColor, float outlineThickness = 2.f)
	{
		sf::RectangleShape background;
		background.setSize({ Bounds.size.x, Bounds.size.y });
		background.setPosition({ Bounds.position.x, Bounds.position.y });
		background.setFillColor(fillColor);
		background.setOutlineColor(outlineColor);
		background.setOutlineThickness(outlineThickness);
		window.draw(background);
	}
};