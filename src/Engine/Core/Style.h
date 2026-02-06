#pragma once

#include <cstdint>

#include <SFML/Graphics/Color.hpp>

// namespace: 상수 집합 (색상 등)
// struct: 설정 가능한 값 (레이아웃 등)

// ============================================================
// 로그 메시지 색상
// ============================================================
namespace LogColor
{
	constexpr uint32_t White = 0xFFFFFFFF;
	constexpr uint32_t Grey = 0xAAAAAAFF;
	constexpr uint32_t DarkGrey = 0x666666FF;

	constexpr uint32_t Info = 0xCCCCCCFF;
	constexpr uint32_t Warning = 0xFFFF00FF;
	constexpr uint32_t Error = 0xFF4444FF;
	constexpr uint32_t Success = 0x44FF44FF;

	constexpr uint32_t Move = 0x888888FF;
	constexpr uint32_t Stairs = 0xFFFF00FF;
	constexpr uint32_t Combat = 0xFF6666FF;
	constexpr uint32_t Item = 0x66CCFFFF;
	constexpr uint32_t Heal = 0x66FF66FF;
}

// ============================================================
// SFML 렌더링용 색상
// ============================================================
namespace Colors
{
	// 기본 색상
	const sf::Color White = sf::Color::White;
	const sf::Color Black = sf::Color::Black;
	const sf::Color Grey = sf::Color(170, 170, 170);
	const sf::Color DarkGrey = sf::Color(102, 102, 102);
	const sf::Color LightGrey = sf::Color(200, 200, 200);

	// 패널 배경/테두리
	namespace Panel
	{
		const sf::Color PortraitBg = sf::Color(30, 30, 50);
		const sf::Color PortraitBorder = sf::Color(60, 60, 90);
		const sf::Color PortraitText = sf::Color(100, 100, 120);

		const sf::Color InfoBg = sf::Color(25, 30, 35);
		const sf::Color InfoBorder = sf::Color(50, 60, 70);

		const sf::Color LogBg = sf::Color(20, 20, 20);
		const sf::Color LogBorder = sf::Color(60, 60, 60);

		const sf::Color MinimapBg = sf::Color(15, 15, 20);
		const sf::Color MinimapBorder = sf::Color(40, 40, 50);
	} 

	// 미니맵 타일
	namespace Minimap
	{
		const sf::Color VisibleFloor = sf::Color(80, 80, 100);
		const sf::Color VisibleWall = sf::Color(40, 40, 50);
		const sf::Color ExploredFloor = sf::Color(40, 40, 50);
		const sf::Color ExploredWall = sf::Color(25, 25, 30);
		const sf::Color Player = sf::Color(255, 255, 100);
	}

	// 게임 월드
	namespace World
	{
		const sf::Color Visible = sf::Color::White;
		const sf::Color Explored = sf::Color(100, 100, 100);
	}

	// 바/게이지
	namespace Bar
	{
		const sf::Color HP = sf::Color(180, 50, 50);
		const sf::Color HPBg = sf::Color(60, 20, 20);
		const sf::Color MP = sf::Color(50, 80, 180);
		const sf::Color MPBg = sf::Color(20, 30, 60);
		const sf::Color Outline = sf::Color(80, 80, 80);
	}

	// 텍스트
	const sf::Color Text = sf::Color(200, 200, 200);
}

// ============================================================
// UI 레이아웃 (인스턴스화 필요)
// ============================================================
struct Style
{
	int FontSize = 14;
	int SmallFontSize = 12;

	float Padding = 8.f;
	float BarHeight = 20.f;
	float BarSpacing = 6.f;
	float LineSpacing = 16.f;
	float SectionSpacing = 12.f;
};