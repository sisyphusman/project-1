#pragma once

#include <algorithm>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "Engine/Entities/CharacterStats.h"
#include "Engine/World/Map.h"
#include "Engine/Entities/Player.h"
#include "Engine/World/DungeonManager.h"
#include "Engine/Systems/FOV.h"
#include "Engine/Systems/Camera.h"
#include "Engine/UI/Panel/PortraitPanel.h"
#include "Engine/UI/Panel/InfoPanel.h"
#include "Engine/UI/Panel/MessageLogPanel.h"
#include "Engine/UI/Panel/MinimapPanel.h"
#include "Engine/Core/Macros.h"
#include "Engine/Systems/CombatSystem.h"
#include "Engine/Systems/DamagePopupSystem.h"
#include "Engine/Systems/TurnSystem.h"
#include "Engine/Data/EnemyCatalog.h"

class Map;
class Player;
class DungeonManager;
class FOV;
class Camera;

class PortraitPanel;
class InfoPanel;
class MessageLogPanel;
class MinimapPanel;

class Game
{
public:
	Game();

	void Run();

private:
	enum class GameFlowState
	{
		MainMenu,
		InGame
	};

	void Init();
	void InitUI();

	void ProcessEvents();
	void ExecuteMainMenu();
	void StartNewRun();
	void CheckStairs();
	void Update(float deltaTime);

	void Render();
	void RenderGameWorld();
	void RenderBottomUI();
	void RenderMainMenu();

	// 지연 생성 필요 -> 스마트 포인터 사용

	// SFML
	sf::RenderWindow Window;
	sf::Font		 GameFont;
	sf::View		 GameView;
	sf::View		 BottomUIView;
	sf::Clock		 GameClock;

	// 월드
	std::unique_ptr<DungeonManager> Dungeon;
	std::unique_ptr<Player>			GamePlayer;
	std::unique_ptr<FOV>			PlayerFOV;
	std::unique_ptr<Camera>			GameCamera;

	// UI 패널
	std::unique_ptr<PortraitPanel>	 Portrait;
	std::unique_ptr<InfoPanel>		 Info;
	std::unique_ptr<MessageLogPanel> Log;
	std::unique_ptr<MinimapPanel>	 Minimap;

	// 시스템
	CombatSystem	  Combat;
	TurnSystem		  Turn;
	DamagePopupSystem DamagePopups;

	CharacterStats MyCharStats;

	// 카탈로그
	EnemyCatalog EnemyDataCatalog;

	// 게임 시작 시 메인메뉴만 렌더링
	GameFlowState FlowState = GameFlowState::MainMenu;

	int MainMenuSelectedIndex = 0;
};
