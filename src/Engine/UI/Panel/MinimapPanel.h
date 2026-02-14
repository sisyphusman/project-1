#pragma once

#include "Engine/UI/Panel/UIPanel.h"
#include "Engine/Core/Style.h"

class Map;
class FOV;

// 미니맵 패널
class MinimapPanel : public UIPanel
{
public:
	MinimapPanel(float x, float y, float width, float height);

	void Update(float deltaTime) override;
	void Render(sf::RenderWindow& window, const sf::Font& font) override;

	// 데이터 소스 연결
	void SetSources(const Map* map, const FOV* fov, const sf::Vector2i* playerPos, const int* level);

private:
	// 원본 데이터 참조
	const Map*			MapRef = nullptr;
	const FOV*			FOVRef = nullptr;
	const sf::Vector2i* PlayerPosRef = nullptr;
	const int*			LevelRef = nullptr;

	// 매 프레임 계산된 결과를 임시 보관
	int PlayerX = 0;
	int PlayerY = 0;
	int CurrentLevel = 1;

	mutable float TileSize = 4.f; // 미니맵의 1타일의 픽셀 수
};