#pragma once

#include <vector>
#include <memory>

#include <SFML/System/Vector2.hpp>

#include "Engine/World/Map.h"

class DungeonManager
{
public:
	DungeonManager(int mapWidth, int mapHeight); // 맵 크기를 저정하고 첫 번째 레벨 생성

	// 다음/이전 레벨로 이동 (성공 시 플레이어 시작 위치 반환)
	bool GoToNextLevel(sf::Vector2i& outPlayerPos);
	bool GoToPrevLevel(sf::Vector2i& outPlayerPos);

	Map&	   GetCurrentMap(); // 현재 레벨의 맵 반환
	int		   GetCurrentLevel() const { return CurrentLevel; }
	const int& GetCurrentLevelRef() const { return CurrentLevel; }

	// 탐험 데이터 저장/복원
	void SaveExploredData(int level, const std::vector<bool>& data);
	bool LoadExploredData(int level, std::vector<bool>& outData) const;

private:
	void GenerateLevel(int level); // 새로운 던전 레벨 생성

	// 모든 층에서 공통으로 사용하는 맵 크기
	int MapWidth;
	int MapHeight;

	// 현재 층 인덱스
	int CurrentLevel = 0;

	std::vector<std::unique_ptr<Map>>				   Levels;
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> StairsPositions;	  // <UpStairs, DownStairs>
	std::vector<std::vector<bool>>					   LevelExploredData; // 층별 탐험 데이터
};