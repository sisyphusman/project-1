#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

class Map;
class Player;
class DungeonManager;
class FOV;
class Camera;

class Game
{
public:
	Game();
	~Game();

	void Run();

private:
	void Init();
	void ProcessEvents();						// 입력 이벤트 처리
	void Update(float deltaTime);
	void Render();								// 화면 렌더링
	void CheckStairs();							// 계단 이동
	void UpdateCamera();

	sf::RenderWindow Window;					// 메인 윈도우
	sf::Font		 GameFont;					// 텍스트 리소스
	sf::View		 GameView;					// 화면 영역
	sf::Clock		 GameClock;					// 델타 타임 계산

	std::unique_ptr<DungeonManager> Dungeon;
	std::unique_ptr<Player>			GamePlayer;
	std::unique_ptr<FOV>			PlayerFOV;
	std::unique_ptr<Camera>			GameCamera;

	// 랜더링 설정 (FHD 1920x1080 기준)
	static constexpr int TileSize = 24;			// 한 타일의 픽셀 크기 (24x24)
	static constexpr int ViewWidthTiles = 80;	// 화면 가로 타일 수
	static constexpr int ViewHeightTiles = 45;	// 화면 세로 타일 수
	static constexpr int FOVRadius = 10;		// 시야 반경
};
