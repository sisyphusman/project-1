#include "Engine/UI/Panel/MinimapPanel.h"

#include "Engine/World/Map.h"
#include "Engine/Systems/FOV.h"

#include <format>

MinimapPanel::MinimapPanel(float x, float y, float width, float height)
	: UIPanel(x, y, width, height)
{
}

void MinimapPanel::SetSources(const Map* map, const FOV* fov, const sf::Vector2i* playerPos, const int* level)
{
	MapRef = map;
	FOVRef = fov;
	PlayerPosRef = playerPos;
	LevelRef = level;
}

void MinimapPanel::Update(float deltaTime)
{
	// 내부에서 직접 업데이트
	if (PlayerPosRef)
	{
		PlayerX = PlayerPosRef->x;
		PlayerY = PlayerPosRef->y;
	}

	if (LevelRef)
	{
		CurrentLevel = *LevelRef;
	}
}

void MinimapPanel::Render(sf::RenderWindow& window, const sf::Font& font)
{
	DrawBackground(window, Colors::Panel::MinimapBg, Colors::Panel::MinimapBorder);

	if (!MapRef || !FOVRef)
	{
		return;
	}

	const Style& s = DefaultStyle;

	// 층 표시
	sf::Text levelText(font, std::format("B{}F", CurrentLevel), s.MinimapLevelFontSize);
	levelText.setFillColor(Colors::Text);
	levelText.setPosition({ Bounds.position.x + s.Padding, Bounds.position.y + s.Padding });
	window.draw(levelText);

	// 미니맵 영역 계산
	float mapAreaX = Bounds.position.x + s.Padding;
	float mapAreaY = Bounds.position.y + s.MinimapHeaderOffsetY;
	float mapAreaWidth = Bounds.size.x - s.Padding * 2.f;
	float mapAreaHeight = Bounds.size.y - (s.MinimapHeaderOffsetY + s.MinimapBottomPadding);

	int mapWidth = MapRef->GetWidth();
	int mapHeight = MapRef->GetHeight();

	// 타일 크기 계산 (가로와 세로중 작은 비율 택함)
	TileSize = std::min(mapAreaWidth / mapWidth, mapAreaHeight / mapHeight);

	// 중앙 정렬 오프셋 (여백이 있을 수 있음)
	float offsetX = mapAreaX + (mapAreaWidth - mapWidth * TileSize) / 2.f;
	float offsetY = mapAreaY + (mapAreaHeight - mapHeight * TileSize) / 2.f;

	// 타일 간격 추가
	sf::RectangleShape tile;
	tile.setSize({ TileSize - s.MinimapTileGap, TileSize - s.MinimapTileGap });

	for (int y = 0; y < mapHeight; ++y)
	{
		for (int x = 0; x < mapWidth; ++x)
		{
			if (!FOVRef->IsExplored(x, y))
			{
				continue;
			}

			const Tile& mapTile = MapRef->GetTile(x, y);
			tile.setPosition({ offsetX + x * TileSize, offsetY + y * TileSize });

			if (FOVRef->IsVisible(x, y))
			{
				// 시야 + 길
				if (mapTile.Walkable)
				{
					tile.setFillColor(Colors::Minimap::VisibleFloor);
				}
				// 시야 + 벽
				else
				{
					tile.setFillColor(Colors::Minimap::VisibleWall);
				}
			}
			else
			{
				// 비가시 + 길
				if (mapTile.Walkable)
				{
					tile.setFillColor(Colors::Minimap::ExploredFloor);
				}
				// 비가시 + 벽
				else
				{
					tile.setFillColor(Colors::Minimap::ExploredWall);
				}
			}

			window.draw(tile);
		}
	}

	// 플레이어 표시
	sf::RectangleShape playerMarker;
	playerMarker.setSize({ TileSize + s.MinimapPlayerMarkerMargin, TileSize + s.MinimapPlayerMarkerMargin });
	playerMarker.setPosition({ offsetX + PlayerX * TileSize - s.MinimapPlayerMarkerMargin, offsetY + PlayerY * TileSize - s.MinimapPlayerMarkerMargin });
	playerMarker.setFillColor(Colors::Minimap::Player);
	window.draw(playerMarker);
}