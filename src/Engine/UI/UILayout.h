#pragma once

#include <cstdint> // uint32_t

// static constexpr: 가독성을 위해서 중괄호가 아닌 등호 초기화 사용

// 게임 레이아웃
struct UILayout
{
	UILayout() = delete;

	// 기준 해상도
	static constexpr int BaseWidth = 1920;
	static constexpr int BaseHeight = 1080;

	// 상하 분할 (게임 영역 / 하단 UI)
	static constexpr float GameAreaRatio = 0.7f;
	static constexpr float BottomUIRatio = 0.3f;

	// 영역 높이 계산
	static constexpr int GameAreaHeight = static_cast<int>(BaseHeight * GameAreaRatio);
	static constexpr int BottomUIHeight = BaseHeight - GameAreaHeight;

	// 하단 UI 4분할 (초상화 / 인포 / 로그 / 맵)
	static constexpr float PortraitRatio = 0.10f;
	static constexpr float InfoRatio = 0.20f;
	static constexpr float LogRatio = 0.40f;
	static constexpr float MinimapRatio = 0.30f;

	static constexpr int PortraitWidth = static_cast<int>(BaseWidth * PortraitRatio);
	static constexpr int InfoWidth = static_cast<int>(BaseWidth * InfoRatio);
	static constexpr int LogWidth = static_cast<int>(BaseWidth * LogRatio);
	static constexpr int MinimapWidth = BaseWidth - PortraitWidth - InfoWidth - LogWidth;

	// 타일 설정
	static constexpr int TileSize = 24;
	static constexpr int ViewWidthTiles = BaseWidth / TileSize;
	static constexpr int ViewHeightTiles = GameAreaHeight / TileSize;

	// 시야 반경
	static constexpr int FOVRadius = 8;

	// 로그 영역 설정
	static constexpr int LogFontSize = 18;
	static constexpr int LogPaddingTop = 10;
	static constexpr int LogPaddingLeft = 15;
	static constexpr int LogLineHeight = LogFontSize + 6;
};

struct LogColor
{
	LogColor() = delete;

	// 기본
	static constexpr uint32_t White = 0xFFFFFFFF;
	static constexpr uint32_t Grey = 0xAAAAAAFF;
	static constexpr uint32_t DarkGrey = 0x666666FF;

	// 시스템 메시지
	static constexpr uint32_t Info = 0xCCCCCCFF;	// 밝은 회색
	static constexpr uint32_t Warning = 0xFFFF00FF; // 노란색
	static constexpr uint32_t Error = 0xFF4444FF;	// 밝은 빨간색
	static constexpr uint32_t Success = 0x44FF44FF; // 밝은 초록색

	// 게임 이벤트
	static constexpr uint32_t Move = 0x888888FF;   // 회색
	static constexpr uint32_t Stairs = 0xFFFF00FF; // 노란색
	static constexpr uint32_t Combat = 0xFF6666FF; // 연한 붉은색
	static constexpr uint32_t Item = 0x66CCFFFF;   // 하늘색
	static constexpr uint32_t Heal = 0x66FF66FF;   // 연한 초록색
};