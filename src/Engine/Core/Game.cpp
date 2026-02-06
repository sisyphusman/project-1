#include "Engine/Core/Game.h"

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

Game::Game()
	: Window(sf::VideoMode::getDesktopMode(), "project-1", sf::Style::None)
	, GameView(sf::FloatRect({ 0.f, 0.f }, { static_cast<float>(UILayout::BaseWidth), static_cast<float>(UILayout::GameAreaHeight) }))
	, BottomUIView(sf::FloatRect({ 0.f, 0.f }, { static_cast<float>(UILayout::BaseWidth), static_cast<float>(UILayout::BottomUIHeight) }))
{
	// 게임 플레이 화면
	GameView.setViewport(sf::FloatRect({ 0.f, 0.f }, { 1.f, UILayout::GameAreaRatio }));

	// 하단 UI
	BottomUIView.setViewport(sf::FloatRect({ 0.f, UILayout::GameAreaRatio }, { 1.f, UILayout::BottomUIRatio }));

	Init();
}

Game::~Game()
{
}

void Game::Init()
{
	// 실패하면 abort()
	GAME_CHECK(GameFont.openFromFile("assets/fonts/Pretendard-Regular.ttf"));

	// 뷰 설정 (해상도 독립적)
	Window.setView(GameView);

	// 맵 생성
	Dungeon = std::make_unique<DungeonManager>(UILayout::ViewWidthTiles, UILayout::ViewHeightTiles);

	// 플레이어 시작 위치 찾기
	Map& currentMap = Dungeon->GetCurrentMap();

	// FOV 시스템 초기화
	PlayerFOV = std::make_unique<FOV>(currentMap.GetWidth(), currentMap.GetHeight());

	// 카메라 초기화
	GameCamera = std::make_unique<Camera>();
	GameCamera->SetNormalZoom(1.0f);
	GameCamera->SetZoomedOutZoom(0.5f);

	// UI 초기화 (패널 생성)
	InitUI();

	// 첫 번째 걸을 수 있는 타일을 찾아 플레이어 배치
	for (int y = 1; y < currentMap.GetHeight() - 1; ++y)
	{
		for (int x = 1; x < currentMap.GetWidth() - 1; ++x)
		{
			if (currentMap.GetTile(x, y).Walkable)
			{
				GamePlayer = std::make_unique<Player>(x, y);

				GameCamera->SetTarget(static_cast<float>(x * UILayout::TileSize), static_cast<float>(y * UILayout::TileSize));

				// 시야 계산
				auto pos = GamePlayer->GetPosition();
				PlayerFOV->Compute(currentMap, pos.x, pos.y, UILayout::FOVRadius);

				// 미니맵 초기화
				Minimap->SetSources(&Dungeon->GetCurrentMap(), PlayerFOV.get(), &GamePlayer->GetPositionRef(), &Dungeon->GetCurrentLevelRef());
				return;
			}
		}
	}
}

void Game::InitUI()
{
	float bottomHeight = static_cast<float>(UILayout::BottomUIHeight);
	float xOffset = 0.f;

	// 초상화 패널
	Portrait = std::make_unique<PortraitPanel>(
		xOffset, 0.f,
		static_cast<float>(UILayout::PortraitWidth), bottomHeight);
	Portrait->LoadPortrait("assets/Portrait.png");
	xOffset += UILayout::PortraitWidth;

	// 인포 패널
	Info = std::make_unique<InfoPanel>(xOffset, 0.f, static_cast<float>(UILayout::InfoWidth), bottomHeight);

	CharacterStats testStats;
	testStats.HP = { 85, 100 };
	testStats.MP = { 15, 20 };
	testStats.Level = 3;
	testStats.STR = 12;
	testStats.DEX = 10;
	testStats.INT = 8;
	Info->SetStats(testStats);
	xOffset += UILayout::InfoWidth;

	// 메시지 로그 패널
	Log = std::make_unique<MessageLogPanel>(xOffset, 0.f, static_cast<float>(UILayout::LogWidth), bottomHeight);
	Log->GetLog().AddMessage("던전에 입장하셨습니다", LogColor::White);
	xOffset += UILayout::LogWidth;

	// 미니맵 패널
	Minimap = std::make_unique<MinimapPanel>(xOffset, 0.f, static_cast<float>(UILayout::MinimapWidth), bottomHeight);
}

void Game::Run()
{
	GameClock.restart();

	while (Window.isOpen())
	{
		float deltaTime = GameClock.restart().asSeconds();
		ProcessEvents();
		Update(deltaTime);
		Render();
	}
}

void Game::ProcessEvents()
{
	while (const std::optional<sf::Event> event = Window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			Window.close();
		}

		if (const auto* key = event->getIf<sf::Event::KeyPressed>())
		{
			int dx = 0;
			int dy = 0;

			switch (key->code)
			{
				case sf::Keyboard::Key::W:
				case sf::Keyboard::Key::Up:
					dy = -1;
					break;
				case sf::Keyboard::Key::S:
				case sf::Keyboard::Key::Down:
					dy = 1;
					break;
				case sf::Keyboard::Key::A:
				case sf::Keyboard::Key::Left:
					dx = -1;
					break;
				case sf::Keyboard::Key::D:
				case sf::Keyboard::Key::Right:
					dx = 1;
					break;
				case sf::Keyboard::Key::Escape:
					Window.close();
					break;
				case sf::Keyboard::Key::Space:
					CheckStairs();
					break;
				case sf::Keyboard::Key::Tab:
					GameCamera->ToggleZoomOut();
					break;
				default:
					break;
			}

			if (dx != 0 || dy != 0)
			{
				if (GamePlayer->TryMove(dx, dy, Dungeon->GetCurrentMap()))
				{
					auto pos = GamePlayer->GetPosition();
					PlayerFOV->Compute(Dungeon->GetCurrentMap(), pos.x, pos.y, UILayout::FOVRadius);
					GameCamera->SetTarget(static_cast<float>(pos.x * UILayout::TileSize), static_cast<float>(pos.y * UILayout::TileSize));
					Log->GetLog().AddMessage("이동했습니다", LogColor::Move);
				}
			}
		}
	}
}

void Game::CheckStairs()
{
	auto		pos = GamePlayer->GetPosition();
	const Tile& tile = Dungeon->GetCurrentMap().GetTile(pos.x, pos.y);

	sf::Vector2i newPos;
	int			 currentLevel = Dungeon->GetCurrentLevel();

	if (tile.Type == TileType::StairDown)
	{
		Dungeon->SaveExploredData(currentLevel, PlayerFOV->GetExploredData());

		if (Dungeon->GoToNextLevel(newPos))
		{
			GamePlayer->SetPosition(newPos.x, newPos.y);

			std::vector<bool> savedData;
			if (Dungeon->LoadExploredData(Dungeon->GetCurrentLevel(), savedData))
			{
				PlayerFOV->SetExploredData(savedData);
			}
			else
			{
				PlayerFOV->Reset();
			}

			PlayerFOV->Compute(Dungeon->GetCurrentMap(), newPos.x, newPos.y, UILayout::FOVRadius);
			Log->GetLog().AddMessage("아래층으로 내려갑니다", LogColor::Stairs);
			GameCamera->SetTarget(static_cast<float>(newPos.x * UILayout::TileSize), static_cast<float>(newPos.y * UILayout::TileSize));
		}
	}
	else if (tile.Type == TileType::StairUp)
	{
		Dungeon->SaveExploredData(currentLevel, PlayerFOV->GetExploredData());

		if (Dungeon->GoToPrevLevel(newPos))
		{
			GamePlayer->SetPosition(newPos.x, newPos.y);

			std::vector<bool> savedData;
			if (Dungeon->LoadExploredData(Dungeon->GetCurrentLevel(), savedData))
			{
				PlayerFOV->SetExploredData(savedData);
			}
			else
			{
				PlayerFOV->Reset();
			}

			PlayerFOV->Compute(Dungeon->GetCurrentMap(), newPos.x, newPos.y, UILayout::FOVRadius);
			Log->GetLog().AddMessage("위층으로 올라갑니다", LogColor::Stairs);
			GameCamera->SetTarget(static_cast<float>(newPos.x * UILayout::TileSize), static_cast<float>(newPos.y * UILayout::TileSize));
		}
	}
}

void Game::Update(float deltaTime)
{
	GameCamera->Update(deltaTime);
	UpdateCamera();

	Portrait->Update(deltaTime);
	Info->Update(deltaTime);
	Log->Update(deltaTime);
	Minimap->Update(deltaTime);
}

void Game::UpdateCamera()
{
	float zoom = GameCamera->GetZoom();
	float viewWidth = UILayout::ViewWidthTiles * UILayout::TileSize / zoom;
	float viewHeight = UILayout::ViewHeightTiles * UILayout::TileSize / zoom;

	GameView.setSize({ viewWidth, viewHeight });
	GameView.setCenter({ GameCamera->GetX(), GameCamera->GetY() });
	Window.setView(GameView);
}

void Game::Render()
{
	Window.clear(Colors::Black);

	RenderGameWorld();
	RenderBottomUI();

	Window.display();
}

void Game::RenderGameWorld()
{
	Window.setView(GameView);

	Map& currentMap = Dungeon->GetCurrentMap();

	sf::Text tileText(GameFont, "", UILayout::TileSize);

	for (int y = 0; y < currentMap.GetHeight(); ++y)
	{
		for (int x = 0; x < currentMap.GetWidth(); ++x)
		{
			const Tile& tile = currentMap.GetTile(x, y);

			if (PlayerFOV->IsVisible(x, y))
			{
				tileText.setString(std::string(1, tile.Glyph));
				tileText.setPosition({ static_cast<float>(x * UILayout::TileSize), static_cast<float>(y * UILayout::TileSize) });
				tileText.setFillColor(Colors::World::Visible);
				Window.draw(tileText);
			}
			else if (PlayerFOV->IsExplored(x, y))
			{
				tileText.setString(std::string(1, tile.Glyph));
				tileText.setPosition({ static_cast<float>(x * UILayout::TileSize), static_cast<float>(y * UILayout::TileSize) });
				tileText.setFillColor(Colors::World::Explored);
				Window.draw(tileText);
			}
		}
	}

	// 플레이어 렌더링
	auto pos = GamePlayer->GetPosition();
	tileText.setString("@");
	tileText.setPosition({ static_cast<float>(pos.x * UILayout::TileSize), static_cast<float>(pos.y * UILayout::TileSize) });
	tileText.setFillColor(Colors::White);
	Window.draw(tileText);
}

void Game::RenderBottomUI()
{
	Window.setView(BottomUIView);

	Portrait->Render(Window, GameFont);
	Info->Render(Window, GameFont);
	Log->Render(Window, GameFont);
	Minimap->Render(Window, GameFont);
}
