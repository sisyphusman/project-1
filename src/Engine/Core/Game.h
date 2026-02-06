#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include "Engine/UI/UILayout.h"

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
	~Game();

	void Run();

private:
	void Init();
	void InitUI();
	void ProcessEvents();
	void Update(float deltaTime);
	void Render();
	void CheckStairs();
	void UpdateCamera();

	void RenderGameWorld();
	void RenderBottomUI();

	sf::RenderWindow Window;
	sf::Font		 GameFont;
	sf::View		 GameView;
	sf::View		 BottomUIView;
	sf::Clock		 GameClock;

	std::unique_ptr<DungeonManager> Dungeon;
	std::unique_ptr<Player>			GamePlayer;
	std::unique_ptr<FOV>			PlayerFOV;
	std::unique_ptr<Camera>			GameCamera;

	// UI 패널
	std::unique_ptr<PortraitPanel>	 Portrait;
	std::unique_ptr<InfoPanel>		 Info;
	std::unique_ptr<MessageLogPanel> Log;
	std::unique_ptr<MinimapPanel>	 Minimap;
};
