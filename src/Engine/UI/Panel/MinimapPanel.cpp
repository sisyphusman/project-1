#include "Engine/UI/Panel/MinimapPanel.h"

#include "Engine/World/Map.h"
#include "Engine/Systems/FOV.h"
#include "Engine/Systems/CombatSystem.h"

#include <format>

MinimapPanel::MinimapPanel(float x, float y, float width, float height)
	: UIPanel(x, y, width, height)
{
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

	// 층 표시
	sf::Text levelText(font, std::format("B{}F", CurrentLevel), UILayout::Tunable::MinimapLevelFontSize);
	levelText.setFillColor(Colors::Text);
	levelText.setPosition({ Bounds.position.x + UILayout::Tunable::Padding, Bounds.position.y + UILayout::Tunable::Padding });
	window.draw(levelText);

	// 미니맵 영역 계산
	float mapAreaX = Bounds.position.x + UILayout::Tunable::Padding;
	float mapAreaY = Bounds.position.y + UILayout::Tunable::MinimapHeaderOffsetY;
	float mapAreaWidth = Bounds.size.x - UILayout::Tunable::Padding * 2.f;
	float mapAreaHeight = Bounds.size.y - (UILayout::Tunable::MinimapHeaderOffsetY + UILayout::Tunable::MinimapBottomPadding);

	int mapWidth = MapRef->GetWidth();
	int mapHeight = MapRef->GetHeight();

	// 타일 크기 계산 (가로와 세로중 작은 비율 택함)
	TileSize = std::min(mapAreaWidth / mapWidth, mapAreaHeight / mapHeight);

	// 중앙 정렬 오프셋 (여백이 있을 수 있음)
	float offsetX = mapAreaX + (mapAreaWidth - mapWidth * TileSize) / 2.f;
	float offsetY = mapAreaY + (mapAreaHeight - mapHeight * TileSize) / 2.f;

	// 타일 간격 추가
	sf::RectangleShape tile;
	tile.setSize({ TileSize - UILayout::Tunable::MinimapTileGap, TileSize - UILayout::Tunable::MinimapTileGap });

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

	// 적 표시 (시야 안에 있고 살아있는 대상만 표시)
	if (Enemies)
	{
		sf::RectangleShape enemyMarker;
		enemyMarker.setSize({ TileSize, TileSize });
		enemyMarker.setFillColor(Colors::Minimap::Enemy);

		for (const CombatEnemy& enemy : *Enemies)
		{
			if (!enemy.bIsAlive)
			{
				continue;
			}

			if (!FOVRef->IsVisible(enemy.Position.x, enemy.Position.y))
			{
				continue;
			}

			enemyMarker.setPosition({ offsetX + enemy.Position.x * TileSize, offsetY + enemy.Position.y * TileSize });
			window.draw(enemyMarker);
		}
	}

	// 플레이어 표시
	sf::RectangleShape playerMarker;
	playerMarker.setSize({ TileSize + UILayout::Tunable::MinimapPlayerMarkerMargin, TileSize + UILayout::Tunable::MinimapPlayerMarkerMargin });
	playerMarker.setPosition({ offsetX + PlayerX * TileSize - UILayout::Tunable::MinimapPlayerMarkerMargin, offsetY + PlayerY * TileSize - UILayout::Tunable::MinimapPlayerMarkerMargin });
	playerMarker.setFillColor(Colors::Minimap::Player);
	window.draw(playerMarker);
}

void MinimapPanel::SetSources(const Map* map, const FOV* fov, const sf::Vector2i* playerPos, const int* level, const std::vector<CombatEnemy>* enemies)
{
	MapRef = map;
	FOVRef = fov;
	PlayerPosRef = playerPos;
	LevelRef = level;
	Enemies = enemies;
}