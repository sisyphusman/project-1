#pragma once

#include <cstdint>

#include <SFML/Graphics/Color.hpp>

// ============================================================
// 로그 메시지 색상
// ============================================================
namespace LogColor
{
	inline constexpr uint32_t White = 0xFFFFFFFF;
	inline constexpr uint32_t Grey = 0xAAAAAAFF;
	inline constexpr uint32_t DarkGrey = 0x666666FF;

	inline constexpr uint32_t Info = 0xCCCCCCFF;
	inline constexpr uint32_t Warning = 0xFFFF00FF;
	inline constexpr uint32_t Error = 0xFF4444FF;
	inline constexpr uint32_t Success = 0x44FF44FF;

	inline constexpr uint32_t Move = 0x888888FF;
	inline constexpr uint32_t Stairs = 0xFFFF00FF;
	inline constexpr uint32_t Combat = 0xFF6666FF;
	inline constexpr uint32_t Item = 0x66CCFFFF;
	inline constexpr uint32_t Heal = 0x66FF66FF;
}

// ============================================================
// SFML 렌더링용 색상
// ============================================================
namespace Colors
{
	// 기본 색상
	inline const sf::Color White = sf::Color::White;
	inline const sf::Color Black = sf::Color::Black;
	inline const sf::Color Red = sf::Color::Red;
	inline const sf::Color Grey = sf::Color(170, 170, 170);
	inline const sf::Color DarkGrey = sf::Color(102, 102, 102);
	inline const sf::Color LightGrey = sf::Color(200, 200, 200);

	// 패널 배경/테두리
	namespace Panel
	{
		inline const sf::Color PortraitBg = sf::Color(30, 30, 50);
		inline const sf::Color PortraitBorder = sf::Color(60, 60, 90);
		inline const sf::Color PortraitText = sf::Color(100, 100, 120);

		inline const sf::Color InfoBg = sf::Color(25, 30, 35);
		inline const sf::Color InfoBorder = sf::Color(50, 60, 70);

		inline const sf::Color LogBg = sf::Color(20, 20, 20);
		inline const sf::Color LogBorder = sf::Color(60, 60, 60);
		inline const sf::Color MinimapBg = sf::Color(15, 15, 20);
		inline const sf::Color MinimapBorder = sf::Color(40, 40, 50);
	}

	// 미니맵 타일
	namespace Minimap
	{
		inline const sf::Color VisibleFloor = sf::Color(80, 80, 100);
		inline const sf::Color VisibleWall = sf::Color(40, 40, 50);
		inline const sf::Color ExploredFloor = sf::Color(40, 40, 50);
		inline const sf::Color ExploredWall = sf::Color(25, 25, 30);
		inline const sf::Color Player = sf::Color(255, 255, 100);
		inline const sf::Color Enemy = sf::Color(255, 0, 0);
	}

	// 게임 월드
	namespace World
	{
		inline const sf::Color Visible = sf::Color::White;
		inline const sf::Color Explored = sf::Color(100, 100, 100);
	}

	// 바/게이지
	namespace Bar
	{
		inline const sf::Color HP = sf::Color(180, 50, 50);
		inline const sf::Color HPBg = sf::Color(60, 20, 20);
		inline const sf::Color MP = sf::Color(50, 80, 180);
		inline const sf::Color MPBg = sf::Color(20, 30, 60);
		inline const sf::Color Outline = sf::Color(80, 80, 80);
	}

	// 텍스트
	inline const sf::Color Text = sf::Color(200, 200, 200);
}

// ============================================================
// UI 레이아웃
// ============================================================

namespace UILayout
{
	namespace Fixed
	{
		inline constexpr int BaseWidth = 1920;
		inline constexpr int BaseHeight = 1080;
		inline constexpr int TileSize = 24;

		inline std::string TitleName = "타이틀";
		inline std::string MenuName0 = "게임시작";
		inline std::string MenuName1 = "종료";
	}

	namespace Tunable
	{
		// 상하 분할 (게임 영역 / 하단 UI)
		inline float GameAreaRatio = 0.7f;

		// 하단 UI 4분할 (초상화 / 인포 / 로그 / 맵)
		inline float PortraitRatio = 0.10f;
		inline float InfoRatio = 0.20f;
		inline float LogRatio = 0.40f;
		inline float MinimapRatio = 0.30f;

		// 시야 반경
		inline int FOVRadius = 8;

		// 로그 영역 설정
		inline int LogFontSize = 18;
		inline int LogPaddingTop = 10;
		inline int LogPaddingLeft = 15;

		// 패널 공통 텍스트/여백
		inline int	 FontSize = 14;
		inline int	 SmallFontSize = 12;
		inline float Padding = 8.f;
		inline float LineSpacing = 16.f;
		inline float SectionSpacing = 12.f;

		// 초상화 패널
		inline int PortraitPlaceholderFontSize = 48;

		// 인포 패널
		inline float BarHeight = 20.f;
		inline float BarSpacing = 6.f;
		inline float InfoStatExtraSpacing = 4.f;
		inline float BarOutlineThickness = 1.f;
		inline float BarTextOffsetY = -2.f;

		// 미니맵 패널
		inline int	 MinimapLevelFontSize = 14;
		inline float MinimapHeaderOffsetY = 30.f;
		inline float MinimapBottomPadding = 10.f;
		inline float MinimapTileGap = 1.f;
		inline float MinimapPlayerMarkerMargin = 1.f;

		// 메인 메뉴 패널
		inline float MainMenuBoxWidthRatio = 0.42f;
		inline float MainMenuBoxHeightRatio = 0.42f;
		inline float MainMenuTitleOffsetYRatio = 0.18f;
		inline float MainMenuStartOffsetYRatio = 0.55f;
		inline float MainMenuQuitOffsetYRatio = 0.72f;
		inline int	 MainMenuTitleFontSize = 72;
		inline int	 MainMenuItemFontSize = 32;
		inline float MainMenuOutlineThickness = 2.f;
	}

	namespace Derived
	{
		// 하단 UI 비율
		inline float BottomUIRatio()
		{
			return 1.f - Tunable::GameAreaRatio;
		}

		// 영역 높이 계산
		inline int GameAreaHeight()
		{
			return static_cast<int>(Fixed::BaseHeight * Tunable::GameAreaRatio);
		}

		// 하단 UI 길이
		inline int BottomUIHeight()
		{
			return Fixed::BaseHeight - GameAreaHeight();
		}

		inline int PortraitWidth()
		{
			return static_cast<int>(Fixed::BaseWidth * Tunable::PortraitRatio);
		}

		inline int InfoWidth()
		{
			return static_cast<int>(Fixed::BaseWidth * Tunable::InfoRatio);
		}

		inline int LogWidth()
		{
			return static_cast<int>(Fixed::BaseWidth * Tunable::LogRatio);
		}

		inline int MinimapWidth()
		{
			return Fixed::BaseWidth - PortraitWidth() - InfoWidth() - LogWidth();
		}

		// 월드 타일 기준 뷰 크기
		inline int ViewWidthTiles()
		{
			return Fixed::BaseWidth / Fixed::TileSize;
		}

		inline int ViewHeightTiles()
		{
			return GameAreaHeight() / Fixed::TileSize;
		}

		// 로그 줄 높이
		inline int LogLineHeight()
		{
			return Tunable::LogFontSize + 6;
		}
	}

	// 옵션/설정 로드 후 유효성 검사
	inline bool IsValid()
	{
		if (!(Tunable::GameAreaRatio > 0.f && Tunable::GameAreaRatio < 1.f))
		{
			return false;
		}

		if (Derived::BottomUIRatio() <= 0.f)
		{
			return false;
		}

		if (Derived::PortraitWidth() + Derived::InfoWidth() + Derived::LogWidth() + Derived::MinimapWidth() != Fixed::BaseWidth)
		{
			return false;
		}

		return true;
	}
}
