#include "Engine/Core/Game.h"

#include "Engine/World/Map.h"
#include "Engine/Entities/Player.h"
#include "Engine/World/DungeonGenerator.h"
#include "Engine/World/DungeonManager.h"
#include "Engine/Systems/FOV.h"
#include "Engine/UI/MessageLog.h"
#include "Engine/Systems/Camera.h"
#include "Engine/Core/Macros.h"

Game::Game()
	: Window(sf::VideoMode::getDesktopMode(), "project-1", sf::Style::None)
	, GameView(sf::FloatRect({ 0.f, 0.f },
		  { static_cast<float>(UILayout::BaseWidth), static_cast<float>(UILayout::GameAreaHeight) }))
	, UIView(sf::FloatRect({ 0.f, 0.f },
		  { static_cast<float>(UILayout::BaseWidth), static_cast<float>(UILayout::BaseHeight) }))
{
	// 게임 플레이 화면은 상단 영역
	GameView.setViewport(sf::FloatRect({ 0.f, 0.f }, { 1.f, UILayout::GameAreaRatio }));
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

	Log = std::make_unique<MessageLog>();
	Log->AddMessage("던전에 입장하셨습니다", LogColor::White);

	// 첫 번째 걸을 수 있는 타일을 찾아 플레이어 배치
	for (int y = 1; y < currentMap.GetHeight() - 1; ++y)
	{
		for (int x = 1; x < currentMap.GetWidth() - 1; ++x)
		{
			if (currentMap.GetTile(x, y).Walkable)
			{
				GamePlayer = std::make_unique<Player>(x, y);

				// 카메라 초기 위치 설정
				GameCamera->SetTarget(static_cast<float>(x * UILayout::TileSize), static_cast<float>(y * UILayout::TileSize));

				// 첫 시야 계산
				auto pos = GamePlayer->GetPosition();
				PlayerFOV->Compute(currentMap, pos.x, pos.y, UILayout::FOVRadius);
				return;
			}
		}
	}
}

void Game::Run()
{
	GameClock.restart();

	// 메인 루프
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
	// while문에서 변수 선언 하는 이유 1. 범위 최소화 2. 이전 값 참조 실수 방지
	while (const std::optional<sf::Event> event = Window.pollEvent())
	{
		// 창 닫기 이벤트
		if (event->is<sf::Event::Closed>())
		{
			Window.close();
		}

		// 키보드 입력 처리
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

			// 이동 입력이 있으면 플레이어 이동
			if (dx != 0 || dy != 0)
			{
				if (GamePlayer->TryMove(dx, dy, Dungeon->GetCurrentMap()))
				{
					// 이동 성공 시 FOV 재계산
					auto pos = GamePlayer->GetPosition();
					PlayerFOV->Compute(Dungeon->GetCurrentMap(), pos.x, pos.y, UILayout::FOVRadius);

					// 카메라 타겟 업데이트
					GameCamera->SetTarget(static_cast<float>(pos.x * UILayout::TileSize), static_cast<float>(pos.y * UILayout::TileSize));

					Log->AddMessage("이동했습니다", LogColor::Move);
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

	// 아래층 계단 위에 있으면 다음 레벨로 이동
	if (tile.Type == TileType::StairDown)
	{
		// 현재 층의 탐험 데이터 저장
		Dungeon->SaveExploredData(currentLevel, PlayerFOV->GetExploredData());

		if (Dungeon->GoToNextLevel(newPos))
		{
			GamePlayer->SetPosition(newPos.x, newPos.y);

			// 새 층의 탐험 데이터 복원
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
			Log->AddMessage("아래층으로 내려갑니다", LogColor::Stairs);

			// 카메라 타겟 업데이트
			GameCamera->SetTarget(static_cast<float>(newPos.x * UILayout::TileSize), static_cast<float>(newPos.y * UILayout::TileSize));
		}
	}
	// 위층 계단 위에 있으면 이전 레벨로 이동
	else if (tile.Type == TileType::StairUp)
	{
		// 현재 층의 탐험 데이터 저장
		Dungeon->SaveExploredData(currentLevel, PlayerFOV->GetExploredData());

		if (Dungeon->GoToPrevLevel(newPos))
		{
			GamePlayer->SetPosition(newPos.x, newPos.y);

			// 이전 층의 탐험 데이터 복원
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
			Log->AddMessage("위층으로 올라갑니다", LogColor::Stairs);

			// 카메라 타겟 업데이트
			GameCamera->SetTarget(static_cast<float>(newPos.x * UILayout::TileSize), static_cast<float>(newPos.y * UILayout::TileSize));
		}
	}
}

void Game::Update(float deltaTime)
{
	GameCamera->Update(deltaTime); // 카메라 계산
	UpdateCamera();				   // 계산 후 View에 적용
}

void Game::UpdateCamera()
{
	// 카메라 위치 줌을 View에 적용
	float zoom = GameCamera->GetZoom();
	float viewWidth = UILayout::ViewWidthTiles * UILayout::TileSize / zoom;
	float viewHeight = UILayout::ViewHeightTiles * UILayout::TileSize / zoom;

	GameView.setSize({ viewWidth, viewHeight });
	GameView.setCenter({ GameCamera->GetX(), GameCamera->GetY() });
	Window.setView(GameView);
}

void Game::Render()
{
	Window.clear(sf::Color::Black);
	Window.setView(GameView);

	Map& currentMap = Dungeon->GetCurrentMap();

	// 타일 텍스트 객체 생성
	sf::Text tileText(GameFont, "", UILayout::TileSize);

	// 맵 타일 랜더링
	for (int y = 0; y < currentMap.GetHeight(); ++y)
	{
		for (int x = 0; x < currentMap.GetWidth(); ++x)
		{
			const Tile& tile = currentMap.GetTile(x, y);

			// 현재 시야에 보이는 타일
			if (PlayerFOV->IsVisible(x, y))
			{
				tileText.setString(std::string(1, tile.Glyph));
				tileText.setPosition({ static_cast<float>(x * UILayout::TileSize), static_cast<float>(y * UILayout::TileSize) });
				tileText.setFillColor(sf::Color::White);
				Window.draw(tileText);
			}
			// 탐험했지만 현재 보이지 않는 타일
			else if (PlayerFOV->IsExplored(x, y))
			{
				tileText.setString(std::string(1, tile.Glyph));
				tileText.setPosition({ static_cast<float>(x * UILayout::TileSize), static_cast<float>(y * UILayout::TileSize) });
				tileText.setFillColor(sf::Color(100, 100, 100)); // 어두운 회색
				Window.draw(tileText);
			}
			// 미탐험 영역은 렌더링하지 않음 (검은색 유지)
		}
	}

	// 플레이어 랜더링
	auto pos = GamePlayer->GetPosition();
	tileText.setString("@");
	tileText.setPosition({ static_cast<float>(pos.x * UILayout::TileSize), static_cast<float>(pos.y * UILayout::TileSize) });
	tileText.setFillColor(sf::Color::White);
	Window.draw(tileText);

	RenderMessageLog();

	Window.display();
}

void Game::RenderMessageLog()
{
	Window.setView(UIView);

	// 로그 영역 배경
	sf::RectangleShape logBackground;
	logBackground.setSize({ static_cast<float>(UILayout::BaseWidth), static_cast<float>(UILayout::LogAreaHeight) });
	logBackground.setPosition({ 0.f, static_cast<float>(UILayout::GameAreaHeight) });
	logBackground.setFillColor(sf::Color(20, 20, 20));
	logBackground.setOutlineColor(sf::Color(60, 60, 60));
	logBackground.setOutlineThickness(2.f); // 경계선 굵기 (바깥쪽)
	Window.draw(logBackground);

	// 로그 엔트리 가져오기
	sf::Text logText(GameFont, "", UILayout::LogFontSize);
	auto	 entries = Log->GetVisibleEntries(UILayout::MaxVisibleLogLines);

	// 첫 로그 라인의 Y 좌표
	float yPos = static_cast<float>(UILayout::GameAreaHeight + UILayout::LogPaddingTop);

	// 로그 메시지 출력
	for (const LogEntry& entry : entries)
	{
		// UTF-8 문자열을 sf::string으로 변환
		logText.setString(sf::String::fromUtf8(entry.text.begin(), entry.text.end()));
		logText.setPosition({ static_cast<float>(UILayout::LogPaddingLeft), yPos });

		// uint32_t -> sf::Color 변환, 상위 비트 빼고 하위 비트만 추출
		logText.setFillColor(sf::Color(static_cast<uint8_t>((entry.color >> 24) & 0xFF), // R
			static_cast<uint8_t>((entry.color >> 16) & 0xFF),							 // G
			static_cast<uint8_t>((entry.color >> 8) & 0xFF),							 // B
			static_cast<uint8_t>(entry.color & 0xFF)));									 // A
		Window.draw(logText);

		yPos += UILayout::LogLineHeight;
	}
}