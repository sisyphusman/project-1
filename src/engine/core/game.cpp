#include "Engine/Core/Game.h"

#include "Engine/World/Map.h"
#include "Engine/Entities/Player.h"
#include "Engine/World/DungeonGenerator.h"
#include "Engine/World/DungeonManager.h"
#include "Engine/Systems/FOV.h"

Game::Game()
	: Window(sf::VideoMode::getDesktopMode(), "project-1", sf::Style::None)
	, GameView(sf::FloatRect({ 0.f, 0.f },
		  {
			  static_cast<float>(ViewWidthTiles * TileSize),
			  static_cast<float>(ViewHeightTiles * TileSize),
		  }))
{
	Init();
}

Game::~Game()
{
}

void Game::Init()
{
	if (!GameFont.openFromFile("C:/Windows/Fonts/arial.ttf"))
	{
		return;
	}

	// 뷰 설정 (해상도 독립적)
	Window.setView(GameView);

	// 맵 생성
	Dungeon = std::make_unique<DungeonManager>(ViewWidthTiles, ViewHeightTiles);

	// 플레이어 시작 위치 찾기
	Map& currentMap = Dungeon->GetCurrentMap();

	// FOV 시스템 초기화
	PlayerFOV = std::make_unique<FOV>(currentMap.GetWidth(), currentMap.GetHeight());

	// 첫 번째 걸을 수 있는 타일을 찾아 플레이어 배치
	for (int y = 1; y < currentMap.GetHeight() - 1; ++y)
	{
		for (int x = 1; x < currentMap.GetWidth() - 1; ++x)
		{
			if (currentMap.GetTile(x, y).Walkable)
			{
				GamePlayer = std::make_unique<Player>(x, y);

				// 첫 시야 계산
				auto pos = GamePlayer->GetPosition();
				PlayerFOV->Compute(currentMap, pos.x, pos.y, FOVRadius);
				return;
			}
		}
	}
}

void Game::Run()
{
	// 메인 루프
	while (Window.isOpen())
	{
		ProcessEvents();
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
					PlayerFOV->Compute(Dungeon->GetCurrentMap(), pos.x, pos.y, FOVRadius);
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

			PlayerFOV->Compute(Dungeon->GetCurrentMap(), newPos.x, newPos.y, FOVRadius);
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

			PlayerFOV->Compute(Dungeon->GetCurrentMap(), newPos.x, newPos.y, FOVRadius);
		}
	}
}
void Game::Update()
{
}

void Game::Render()
{
	Window.clear(sf::Color::Black);

	Map& currentMap = Dungeon->GetCurrentMap();

	// 타일 텍스트 객체 생성
	sf::Text tileText(GameFont, "", TileSize);

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
				tileText.setPosition({ static_cast<float>(x * TileSize), static_cast<float>(y * TileSize) });
				tileText.setFillColor(sf::Color::White);
				Window.draw(tileText);
			}
			// 탐험했지만 현재 보이지 않는 타일
			else if (PlayerFOV->IsExplored(x, y))
			{
				tileText.setString(std::string(1, tile.Glyph));
				tileText.setPosition({ static_cast<float>(x * TileSize), static_cast<float>(y * TileSize) });
				tileText.setFillColor(sf::Color(100, 100, 100)); // 어두운 회색
				Window.draw(tileText);
			}
			// 미탐험 영역은 렌더링하지 않음 (검은색 유지)
		}
	}

	// 플레이어 랜더링
	auto pos = GamePlayer->GetPosition();
	tileText.setString("@");
	tileText.setPosition({ static_cast<float>(pos.x * TileSize), static_cast<float>(pos.y * TileSize) });
	tileText.setFillColor(sf::Color::White);
	Window.draw(tileText);

	Window.display();
}
