#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class Map;
class Player;

class Game
{
public:
	Game();
	~Game();

	void Run();

private:
	void Init();
	void ProcessEvents();
	void Update();
	void Render();

	sf::RenderWindow Window;
	sf::Font		 GameFont;
	sf::View		 GameView;

	// 타일 맵
	std::unique_ptr<Map>	GameMap;
	std::unique_ptr<Player> GamePlayer;

	// 랜더링 설정 (FHD 1920x1080 기준)
	static constexpr int TileSize = 24;		   // 한 타일의 픽셀 크기 (24x24)
	static constexpr int ViewWidthTiles = 80;  // 화면 가로 타일 수
	static constexpr int ViewHeightTiles = 45; // 화면 세로 타일 수
};
