#pragma once

#include <cstdint> // uint32_t

// static constexpr: 가독성을 위해서 중괄호가 아닌 등호 초기화 사용

// 게임 레이아웃 (픽셀 기준)
struct UILayout
{
	UILayout() = delete;

	// 기준 해상도
	static constexpr int BaseWidth = 1920;
	static constexpr int BaseHeight = 1080;

	// 상하 분할 (게임 영역 / 로그 영역)
	static constexpr float GameAreaRatio = 0.7f;
	static constexpr float LogAreaRatio = 0.3f;

	// 영역 높이 계산
	static constexpr int GameAreaHeight = static_cast<int>(BaseHeight * GameAreaRatio); // 756
	static constexpr int LogAreaHeight = BaseHeight - GameAreaHeight;					// 324

	// 하단 UI 3분할 (초상화 / 로그 / 스탯)
	static constexpr float PortraitRatio = 0.25f;
	static constexpr float LogRatio = 0.50f;
	static constexpr float StatusRatio = 0.25f;
	
	static constexpr int   PortraitWidth = static_cast<int>(BaseWidth * PortraitRatio);
	static constexpr float LogWidth = static_cast<int>(BaseWidth * LogRatio);
	static constexpr float StatusWidth = static_cast<int>(BaseWidth * StatusRatio);


	// 타일 설정
	static constexpr int TileSize = 24;
	static constexpr int ViewWidthTiles = BaseWidth / TileSize;		  // 80
	static constexpr int ViewHeightTiles = GameAreaHeight / TileSize; // 31

	// 시야 반경
	static constexpr int FOVRadius = 8;

	// 로그 영역 설정
	static constexpr int LogFontSize = 20;
	static constexpr int LogPaddingTop = 10;
	static constexpr int LogPaddingLeft = BaseWidth / 3;
	static constexpr int LogLineHeight = LogFontSize + 4;
	static constexpr int MaxVisibleLogLines = (LogAreaHeight - LogPaddingTop * 2) / LogLineHeight;
};

// 로그 메시지 색상 (RGBA)
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
	static constexpr uint32_t Move = 0xAAAAAAFF;   // 회색
	static constexpr uint32_t Stairs = 0xFFFF00FF; // 노란색
	static constexpr uint32_t Combat = 0xFF6666FF; // 연한 붉은색
	static constexpr uint32_t Item = 0x66CCFFFF;   // 하늘색
	static constexpr uint32_t Heal = 0x66FF66FF;   // 연한 초록색
};