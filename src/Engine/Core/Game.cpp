#include "Engine/Core/Game.h"

Game::Game()
	: Window(sf::VideoMode::getDesktopMode(), "project-1", sf::Style::None)
	, GameView(sf::FloatRect({ 0.f, 0.f }, { static_cast<float>(UILayout::Fixed::BaseWidth), static_cast<float>(UILayout::Derived::GameAreaHeight()) }))
	, BottomUIView(sf::FloatRect({ 0.f, 0.f }, { static_cast<float>(UILayout::Fixed::BaseWidth), static_cast<float>(UILayout::Derived::BottomUIHeight()) }))
{
	// 게임 플레이 화면
	GameView.setViewport(sf::FloatRect({ 0.f, 0.f }, { 1.f, UILayout::Tunable::GameAreaRatio }));

	// 하단 UI
	BottomUIView.setViewport(sf::FloatRect({ 0.f, UILayout::Tunable::GameAreaRatio }, { 1.f, UILayout::Derived::BottomUIRatio() }));

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
	Dungeon = std::make_unique<DungeonManager>(UILayout::Derived::ViewWidthTiles(), UILayout::Derived::ViewHeightTiles());

	// 플레이어 시작 위치 찾기
	Map& currentMap = Dungeon->GetCurrentMap();

	// FOV 시스템 초기화
	PlayerFOV = std::make_unique<FOV>(currentMap.GetWidth(), currentMap.GetHeight());

	// 카메라 초기화
	GameCamera = std::make_unique<Camera>();

	// UI 초기화 (패널 생성)
	InitUI();

	// 컴뱃 시스템 초기화
	Combat.Reset();
	Turn.Reset();

	// 첫 번째 걸을 수 있는 타일을 찾아 플레이어 배치
	for (int y = 1; y < currentMap.GetHeight() - 1; ++y)
	{
		for (int x = 1; x < currentMap.GetWidth() - 1; ++x)
		{
			if (currentMap.GetTile(x, y).Walkable)
			{
				GamePlayer = std::make_unique<Player>(x, y);

				GameCamera->SetTarget(static_cast<float>(x * UILayout::Fixed::TileSize), static_cast<float>(y * UILayout::Fixed::TileSize));

				// 시야 계산
				auto pos = GamePlayer->GetPosition();
				PlayerFOV->Compute(currentMap, pos.x, pos.y, UILayout::Tunable::FOVRadius);

				// 미니맵 초기화
				Minimap->SetSources(&Dungeon->GetCurrentMap(), PlayerFOV.get(), &GamePlayer->GetPositionRef(), &Dungeon->GetCurrentLevelRef());

				if (Combat.SpawnTestEnemy(currentMap, pos))
				{
					std::vector<std::string> discoveredMessages;
					Turn.CollectNewVisibleEnemyMessages(Combat, *PlayerFOV, discoveredMessages);
					for (const std::string& messages : discoveredMessages)
					{
						Log->GetLog().AddMessage(messages, LogColor::Combat);
					}
				}
				return;
			}
		}
	}
}

void Game::InitUI()
{
	float bottomHeight = static_cast<float>(UILayout::Derived::BottomUIHeight());
	float xOffset = 0.f;

	// 초상화 패널
	Portrait = std::make_unique<PortraitPanel>(xOffset, 0.f, static_cast<float>(UILayout::Derived::PortraitWidth()), bottomHeight);
	Portrait->LoadPortrait("assets/Portrait.png");
	xOffset += UILayout::Derived::PortraitWidth();

	// 인포 패널
	Info = std::make_unique<InfoPanel>(xOffset, 0.f, static_cast<float>(UILayout::Derived::InfoWidth()), bottomHeight);

	// 내 캐릭 임시 설정
	MyCharStats.HP = { 85, 100 };
	MyCharStats.MP = { 15, 20 };
	MyCharStats.Level = 3;
	MyCharStats.STR = 12;
	MyCharStats.DEX = 10;
	MyCharStats.INT = 8;
	Info->SetSource(&MyCharStats);
	xOffset += UILayout::Derived::InfoWidth();

	// 메시지 로그 패널
	Log = std::make_unique<MessageLogPanel>(xOffset, 0.f, static_cast<float>(UILayout::Derived::LogWidth()), bottomHeight);
	Log->GetLog().AddMessage("던전에 입장하셨습니다", LogColor::White);
	xOffset += UILayout::Derived::LogWidth();

	// 미니맵 패널
	Minimap = std::make_unique<MinimapPanel>(xOffset, 0.f, static_cast<float>(UILayout::Derived::MinimapWidth()), bottomHeight);
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
				sf::Vector2i currentPos = GamePlayer->GetPosition();

				TurnResult turnResult;

				if (Turn.ExecutePlayerTurn(Combat, Dungeon->GetCurrentMap(), currentPos, dx, dy, MyCharStats, turnResult))
				{
					if (turnResult.bPlayerMoved)
					{
						GamePlayer->SetPosition(turnResult.PlayerNextPosition.x, turnResult.PlayerNextPosition.y);
					}

					for (const std::string& message : turnResult.Messages)
					{
						Log->GetLog().AddMessage(message, message == "이동했습니다" ? LogColor::Move : LogColor::Combat);
					}

					for (const CombatDamageEvent& damageEvent : turnResult.DamageEvents)
					{
						DamagePopups.AddAtTile(damageEvent.TilePosition.x, damageEvent.TilePosition.y, damageEvent.Damage,
							damageEvent.bFromPlayer ? Colors::Red : Colors::White);
					}

					auto pos = GamePlayer->GetPosition();
					PlayerFOV->Compute(Dungeon->GetCurrentMap(), pos.x, pos.y, UILayout::Tunable::FOVRadius);

					std::vector<std::string> discoverdMessages;
					Turn.CollectNewVisibleEnemyMessages(Combat, *PlayerFOV, discoverdMessages);
					for (const std::string& messages : discoverdMessages)
					{
						Log->GetLog().AddMessage(messages, LogColor::Combat);
					}

					GameCamera->SetTarget(static_cast<float>(pos.x * UILayout::Fixed::TileSize), static_cast<float>(pos.y * UILayout::Fixed::TileSize));
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

	// 다음 층 이동
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

			PlayerFOV->Compute(Dungeon->GetCurrentMap(), newPos.x, newPos.y, UILayout::Tunable::FOVRadius);
			Minimap->SetSources(&Dungeon->GetCurrentMap(), PlayerFOV.get(), &GamePlayer->GetPositionRef(), &Dungeon->GetCurrentLevelRef());
			Log->GetLog().AddMessage("아래층으로 내려갑니다", LogColor::Stairs);
			GameCamera->SetTarget(static_cast<float>(newPos.x * UILayout::Fixed::TileSize), static_cast<float>(newPos.y * UILayout::Fixed::TileSize));

			Combat.Reset();
			Turn.Reset();

			if (Combat.SpawnTestEnemy(Dungeon->GetCurrentMap(), newPos))
			{
				std::vector<std::string> discoveredMessages;
				Turn.CollectNewVisibleEnemyMessages(Combat, *PlayerFOV, discoveredMessages);
				for (const std::string& message : discoveredMessages)
				{
					Log->GetLog().AddMessage(message, LogColor::Combat);
				}
			}
		}
	}
	// 이전 층 이동
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

			PlayerFOV->Compute(Dungeon->GetCurrentMap(), newPos.x, newPos.y, UILayout::Tunable::FOVRadius);
			Minimap->SetSources(&Dungeon->GetCurrentMap(), PlayerFOV.get(), &GamePlayer->GetPositionRef(), &Dungeon->GetCurrentLevelRef());
			Log->GetLog().AddMessage("위층으로 올라갑니다", LogColor::Stairs);
			GameCamera->SetTarget(static_cast<float>(newPos.x * UILayout::Fixed::TileSize), static_cast<float>(newPos.y * UILayout::Fixed::TileSize));

			Combat.Reset();
			Turn.Reset();

			if (Combat.SpawnTestEnemy(Dungeon->GetCurrentMap(), newPos))
			{
				std::vector<std::string> discoveredMessages;
				Turn.CollectNewVisibleEnemyMessages(Combat, *PlayerFOV, discoveredMessages);
				for (const std::string& message : discoveredMessages)
				{
					Log->GetLog().AddMessage(message, LogColor::Combat);
				}
			}
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
	DamagePopups.Update(deltaTime);
}

void Game::UpdateCamera()
{
	float zoom = GameCamera->GetZoom();
	float viewWidth = UILayout::Derived::ViewWidthTiles() * UILayout::Fixed::TileSize / zoom;
	float viewHeight = UILayout::Derived::ViewHeightTiles() * UILayout::Fixed::TileSize / zoom;

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

	sf::Text tileText(GameFont, "", UILayout::Fixed::TileSize);

	for (int y = 0; y < currentMap.GetHeight(); ++y)
	{
		for (int x = 0; x < currentMap.GetWidth(); ++x)
		{
			const Tile& tile = currentMap.GetTile(x, y);

			if (PlayerFOV->IsVisible(x, y))
			{
				tileText.setString(std::string(1, tile.Glyph));
				tileText.setPosition({ static_cast<float>(x * UILayout::Fixed::TileSize), static_cast<float>(y * UILayout::Fixed::TileSize) });
				tileText.setFillColor(Colors::World::Visible);
				Window.draw(tileText);
			}
			else if (PlayerFOV->IsExplored(x, y))
			{
				tileText.setString(std::string(1, tile.Glyph));
				tileText.setPosition({ static_cast<float>(x * UILayout::Fixed::TileSize), static_cast<float>(y * UILayout::Fixed::TileSize) });
				tileText.setFillColor(Colors::World::Explored);
				Window.draw(tileText);
			}
		}
	}

	for (const CombatEnemy& enemy : Combat.GetEnemies())
	{
		if (!enemy.bIsAlive)
		{
			continue;
		}

		if (!PlayerFOV->IsVisible(enemy.Position.x, enemy.Position.y))
		{
			continue;
		}

		tileText.setString(std::string(1, enemy.Glyph));
		tileText.setPosition({ static_cast<float>(enemy.Position.x * UILayout::Fixed::TileSize),
			static_cast<float>(enemy.Position.y) * UILayout::Fixed::TileSize });
		tileText.setFillColor(Colors::Red);
		Window.draw(tileText);

		DamagePopups.Render(Window, GameFont);
	}

	// 플레이어 렌더링
	auto pos = GamePlayer->GetPosition();
	tileText.setString("@");
	tileText.setPosition({ static_cast<float>(pos.x * UILayout::Fixed::TileSize), static_cast<float>(pos.y * UILayout::Fixed::TileSize) });
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
