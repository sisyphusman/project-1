#include "Engine/Core/Game.h"

// View 크기를 설정
Game::Game()
	: Window(sf::VideoMode::getDesktopMode(), "project-1", sf::Style::None)
	, GameView(sf::FloatRect({ 0.f, 0.f }, { static_cast<float>(UILayout::Fixed::BaseWidth), static_cast<float>(UILayout::Derived::GameAreaHeight()) }))
	, BottomUIView(sf::FloatRect({ 0.f, 0.f }, { static_cast<float>(UILayout::Fixed::BaseWidth), static_cast<float>(UILayout::Derived::BottomUIHeight()) }))
{
	// 윈도우에서 차지할 비율 설정
	GameView.setViewport(sf::FloatRect({ 0.f, 0.f }, { 1.f, UILayout::Tunable::GameAreaRatio }));
	BottomUIView.setViewport(sf::FloatRect({ 0.f, UILayout::Tunable::GameAreaRatio }, { 1.f, UILayout::Derived::BottomUIRatio() }));

	Init();
}

Game::~Game()
{
}

////////////////////////////////////////////////////////////
// 첫 초기화
////////////////////////////////////////////////////////////
void Game::Init()
{
	// 폰트 파일 로드
	GAME_CHECK(GameFont.openFromFile("assets/fonts/Pretendard-Regular.ttf"));
	GAME_DEBUG_LOG("init", "폰트 파일 로드 완료");

	// 뷰 설정 (해상도 독립적)
	Window.setView(GameView);

	// UI 초기화 (패널 생성)
	InitUI();

	// 에너미 JSON 로드
	std::string enemyDataError;
	GAME_CHECK(EnemyDataCatalog.LoadFromJsonFile("assets/data/enemies.json", enemyDataError));
	GAME_DEBUG_LOG("init", "에너미 JSON 데이터 로드 완료");

	// CombatSystem에 카탈로그 데이터 주소 공유
	Combat.SetEnemyCatalog(&EnemyDataCatalog);

	// 메인 메뉴 상태
	FlowState = GameFlowState::MainMenu;
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

////////////////////////////////////////////////////////////
// 게임 메인 로직
////////////////////////////////////////////////////////////
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
	GAME_CHECK(Dungeon != nullptr || FlowState == GameFlowState::MainMenu);
	while (const std::optional<sf::Event> event = Window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			Window.close();
		}

		if (const auto* key = event->getIf<sf::Event::KeyPressed>())
		{
			if (FlowState == GameFlowState::MainMenu)
			{
				switch (key->code)
				{
					case sf::Keyboard::Key::Enter:
						StartNewRun();
						break;
					case sf::Keyboard::Key::Escape:
						Window.close();
						break;
					default:
						break;
				}

				continue;
			}

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
				case sf::Keyboard::Key::Q:
					dx = -1;
					dy = -1;
					break;
				case sf::Keyboard::Key::E:
					dx = 1;
					dy = -1;
					break;
				case sf::Keyboard::Key::Z:
					dx = -1;
					dy = 1;
					break;
				case sf::Keyboard::Key::C:
					dx = 1;
					dy = 1;
					break;
				default:
					break;
			}

			// 이동 및 전투 처리
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
						Log->GetLog().AddMessage(message, LogColor::Combat);
					}

					for (const CombatDamageEvent& damageEvent : turnResult.DamageEvents)
					{
						DamagePopups.AddAtTile(damageEvent.TilePosition.x, damageEvent.TilePosition.y, damageEvent.Damage,
							damageEvent.bFromPlayer ? Colors::Red : Colors::White);
					}

					auto pos = GamePlayer->GetPosition();
					PlayerFOV->Compute(Dungeon->GetCurrentMap(), pos.x, pos.y, UILayout::Tunable::FOVRadius);

					std::vector<std::string> discoveredMessages;
					Turn.CollectNewVisibleEnemyMessages(Combat, *PlayerFOV, discoveredMessages);
					for (const std::string& message : discoveredMessages)
					{
						Log->GetLog().AddMessage(message, LogColor::Combat);
					}

					GameCamera->SetTarget(static_cast<float>(pos.x * UILayout::Fixed::TileSize), static_cast<float>(pos.y * UILayout::Fixed::TileSize));
				}
			}
		}
	}
}

void Game::StartNewRun()
{

	// 던전 생성
	Dungeon = std::make_unique<DungeonManager>(UILayout::Derived::ViewWidthTiles(), UILayout::Derived::ViewHeightTiles());

	// 플레이어가 시작할 맵 로드
	Map& currentMap = Dungeon->GetCurrentMap();

	// FOV 시스템 초기화
	PlayerFOV = std::make_unique<FOV>(currentMap.GetWidth(), currentMap.GetHeight());

	// 카메라 초기화
	GameCamera = std::make_unique<Camera>();

	// 컴뱃 시스템 초기화
	Combat.Reset();
	Turn.Reset();
	DamagePopups = DamagePopupSystem();

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

				// 에너미 스폰
				if (Combat.SpawnTestEnemy(currentMap, pos))
				{
					std::vector<std::string> discoveredMessages;
					Turn.CollectNewVisibleEnemyMessages(Combat, *PlayerFOV, discoveredMessages);
					for (const std::string& message : discoveredMessages)
					{
						Log->GetLog().AddMessage(message, LogColor::Combat);
					}
				}

				// 미니맵 초기화
				Minimap->SetSources(&Dungeon->GetCurrentMap(), PlayerFOV.get(), &GamePlayer->GetPositionRef(), &Dungeon->GetCurrentLevelRef(),
					&Combat.GetEnemies());

				// 메인 메뉴에서 게임 플레이 상태로 전환
				FlowState = GameFlowState::InGame;
				return;
			}
		}
	}
}

void Game::CheckStairs()
{
	GAME_CHECK(Dungeon != nullptr && GamePlayer != nullptr && PlayerFOV != nullptr);

	auto		pos = GamePlayer->GetPosition();
	const Tile& tile = Dungeon->GetCurrentMap().GetTile(pos.x, pos.y);

	sf::Vector2i newPos;
	int			 currentLevel = Dungeon->GetCurrentLevel();

	// 다음 층 이동
	if (tile.Type == TileType::StairDown)
	{
		Dungeon->SaveExploredData(currentLevel, PlayerFOV->GetExploredData());
		Dungeon->SaveCombatState(currentLevel, Combat);

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

			// 층의 데이터가 저장되어 있다면 로드
			if (!Dungeon->LoadCombatState(Dungeon->GetCurrentLevel(), Combat))
			{
				// 첫 방문이면 신규 에너미 생성
				Combat.Reset();
				Combat.SpawnTestEnemy(Dungeon->GetCurrentMap(), newPos);
			}

			Turn.Reset();
			PlayerFOV->Compute(Dungeon->GetCurrentMap(), newPos.x, newPos.y, UILayout::Tunable::FOVRadius);
			Minimap->SetSources(&Dungeon->GetCurrentMap(), PlayerFOV.get(), &GamePlayer->GetPositionRef(), &Dungeon->GetCurrentLevelRef(),
				&Combat.GetEnemies());
			Log->GetLog().AddMessage("아래층으로 내려갑니다", LogColor::Stairs);
			GameCamera->SetTarget(static_cast<float>(newPos.x * UILayout::Fixed::TileSize), static_cast<float>(newPos.y * UILayout::Fixed::TileSize));

			// 현재 시야에 포착된 에너미 메시지 수집 후 출력
			std::vector<std::string> discoveredMessages;
			Turn.CollectNewVisibleEnemyMessages(Combat, *PlayerFOV, discoveredMessages);
			for (const std::string& message : discoveredMessages)
			{
				Log->GetLog().AddMessage(message, LogColor::Combat);
			}
		}
	}
	// 이전 층 이동
	else if (tile.Type == TileType::StairUp)
	{
		Dungeon->SaveExploredData(currentLevel, PlayerFOV->GetExploredData());
		Dungeon->SaveCombatState(currentLevel, Combat);

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

			// 층의 데이터가 저장되어 있다면 로드
			if (!Dungeon->LoadCombatState(Dungeon->GetCurrentLevel(), Combat))
			{
				// 첫 방문이면 신규 에너미 생성
				Combat.Reset();
				Combat.SpawnTestEnemy(Dungeon->GetCurrentMap(), newPos);
			}

			Turn.Reset();
			PlayerFOV->Compute(Dungeon->GetCurrentMap(), newPos.x, newPos.y, UILayout::Tunable::FOVRadius);
			Minimap->SetSources(&Dungeon->GetCurrentMap(), PlayerFOV.get(), &GamePlayer->GetPositionRef(), &Dungeon->GetCurrentLevelRef(),
				&Combat.GetEnemies());
			Log->GetLog().AddMessage("위층으로 올라갑니다", LogColor::Stairs);
			GameCamera->SetTarget(static_cast<float>(newPos.x * UILayout::Fixed::TileSize), static_cast<float>(newPos.y * UILayout::Fixed::TileSize));

			// 현재 시야에 포착된 에너미 메시지 수집 후 출력
			std::vector<std::string> discoveredMessages;
			Turn.CollectNewVisibleEnemyMessages(Combat, *PlayerFOV, discoveredMessages);
			for (const std::string& message : discoveredMessages)
			{
				Log->GetLog().AddMessage(message, LogColor::Combat);
			}
		}
	}
}

void Game::Update(float deltaTime)
{
	if (FlowState != GameFlowState::InGame)
	{
		return;
	}

	GameCamera->Update(deltaTime, GameView, static_cast<float>(UILayout::Derived::ViewWidthTiles() * UILayout::Fixed::TileSize),
		static_cast<float>(UILayout::Derived::ViewHeightTiles() * UILayout::Fixed::TileSize));
	Minimap->Update(deltaTime);
	DamagePopups.Update(deltaTime);
}

////////////////////////////////////////////////////////////
// 화면 렌더링
////////////////////////////////////////////////////////////
void Game::Render()
{
	Window.clear(Colors::Black);

	if (FlowState == GameFlowState::MainMenu)
	{
		RenderMainMenu();
	}
	else
	{
		RenderGameWorld();
		RenderBottomUI();
	}

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
	}

	DamagePopups.Render(Window, GameFont);

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

void Game::RenderMainMenu()
{
	// 메뉴는 전체 화면 기준 렌더링
	Window.setView(Window.getDefaultView());

	const sf::View	   defaultView = Window.getDefaultView();
	const sf::Vector2f viewSize = defaultView.getSize();
	const sf::Vector2f viewCenter = defaultView.getCenter();

	// 메뉴 박스 크기/위치를 계산
	const float		   menuBoxWidth = viewSize.x * UILayout::Tunable::MainMenuBoxWidthRatio;
	const float		   menuBoxHeight = viewSize.x * UILayout::Tunable::MainMenuBoxWidthRatio;
	const sf::Vector2f menuBoxPosition = { viewCenter.x - (menuBoxWidth * 0.5f), viewCenter.y - (menuBoxHeight * 0.5f) };

	sf::RectangleShape menuBox({ menuBoxWidth, menuBoxHeight });
	menuBox.setPosition(menuBoxPosition);
	menuBox.setFillColor(Colors::Panel::LogBg);
	menuBox.setOutlineThickness(UILayout::Tunable::MainMenuOutlineThickness);
	menuBox.setOutlineColor(Colors::Panel::LogBorder);

	const unsigned int titleFontSize = static_cast<unsigned int>(std::max(12, UILayout::Tunable::MainMenuTitleFontSize));
	const unsigned int itemFontSize = static_cast<unsigned int>(std::max(12, UILayout::Tunable::MainMenuItemFontSize));

	sf::Text titleText(GameFont, sf::String::fromUtf8(UILayout::Fixed::TitleName.begin(), UILayout::Fixed::TitleName.end()), titleFontSize);
	titleText.setFillColor(Colors::White);
	titleText.setStyle(sf::Text::Bold);

	sf::Text startText(GameFont, "Enter - Start", itemFontSize);
	startText.setFillColor(Colors::Text);

	sf::Text quitText(GameFont, "Esc - Quit", itemFontSize);
	quitText.setFillColor(Colors::Text);

	// x 축은 중앙 정렬, y 축은 비율 오프셋 사용, 텍스트의 내부 기준점 차이 제거
	auto CenterTextOnX = [menuBoxPosition, menuBoxWidth, menuBoxHeight](sf::Text& text, float yRatio) {
		// 텍스트가 차지하는 공간, 박스에서 텍스트 공간을 뺀 값의 절반을 이용하여 중앙 정렬
		const sf::FloatRect localBounds = text.getLocalBounds();

		// 박스 왼쪽 + 남는 여백의 절반 + 로컬 오프셋 보정 (글자가 바운딩 박스에서 오른쪽으로 밀려서 시작하기 때문)
		const float x = menuBoxPosition.x + ((menuBoxWidth - localBounds.size.x) * 0.5f) - localBounds.position.x;
		const float y = menuBoxPosition.y + (yRatio * menuBoxHeight) - localBounds.position.y;
		text.setPosition({ x, y });
	};

	CenterTextOnX(titleText, UILayout::Tunable::MainMenuTitleOffsetYRatio);
	CenterTextOnX(startText, UILayout::Tunable::MainMenuStartOffsetYRatio);
	CenterTextOnX(quitText, UILayout::Tunable::MainMenuQuitOffsetYRatio);

	Window.draw(menuBox);
	Window.draw(titleText);
	Window.draw(startText);
	Window.draw(quitText);
}