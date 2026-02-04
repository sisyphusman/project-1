#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include "Engine/UI/UILayout.h"

class Map;
class Player;
class DungeonManager;
class FOV;
class Camera;
class MessageLog;

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

	void RenderMessageLog();					// UI 렌더링

	sf::RenderWindow Window;					// 메인 윈도우
	sf::Font		 GameFont;					// 텍스트 리소스
	sf::View		 GameView;					// 화면 영역
	sf::View		 UIView;					// UI 영역
	sf::Clock		 GameClock;					// 델타 타임 계산

	std::unique_ptr<DungeonManager> Dungeon;
	std::unique_ptr<Player>			GamePlayer;
	std::unique_ptr<FOV>			PlayerFOV;
	std::unique_ptr<Camera>			GameCamera;
	std::unique_ptr<MessageLog>     Log;
};
