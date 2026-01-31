#include "Engine/Core/Game.h"
#include "Engine/World/Map.h"
#include "Engine/Entities/Player.h"
#include "Engine/World/DungeonGenerator.h"
#include "Engine/World/DungeonManager.h"

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

	// 맵 설정
	Dungeon = std::make_unique<DungeonManager>(ViewWidthTiles, ViewHeightTiles);

	//// 던전 생성
	// DungeonGenerator generator;
	// generator.Generate(*GameMap, 10, 5, 12);

	//// 플레이어 생성
	// const auto& rooms = generator.GetRooms();
	// if (!rooms.empty())
	//{
	//	GamePlayer = std::make_unique<Player>(rooms[0].CenterX(), rooms[0].CenterY());
	// }

	// 플레이어 시작 위치 찾기
	Map& currentMap = Dungeon->GetCurrentMap();

	// 첫 번째 걸을 수 있는 타일을 찾아 플레이어 배치
	for (int y = 1; y < currentMap.GetHeight() - 1; ++y)
	{
		for (int x = 1; x < currentMap.GetWidth() - 1; ++x)
		{
			if (currentMap.GetTile(x, y).Walkable)
			{
				GamePlayer = std::make_unique<Player>(x, y);
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
				case sf::Keyboard::Key::Period:
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
					{
						CheckStairs();
					}
					break;
				default:
					break;
			}

			// 이동 입력이 있으면 플레이어 이동
			if (dx != 0 || dy != 0)
			{
				GamePlayer->TryMove(dx, dy, Dungeon->GetCurrentMap());
			}
		}
	}
}

void Game::CheckStairs()
{
	auto		pos = GamePlayer->GetPosition();
	const Tile& tile = Dungeon->GetCurrentMap().GetTile(pos.x, pos.y);

	sf::Vector2i newPos;

	// 아래층 계단 위에 있으면 다음 레벨로 이동
	if (tile.Type == TileType::StairDown)
	{
		if (Dungeon->GoToNextLevel(newPos))
		{
			GamePlayer->SetPosition(newPos.x, newPos.y);
		}
	}

	// 위층 계단 위에 있으면 이전 레벨로 이동
	else if (tile.Type == TileType::StairUp)
	{
		if (Dungeon->GoToPrevLevel(newPos))
		{
			GamePlayer->SetPosition(newPos.x, newPos.y);
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

	// 맵 랜더링
	sf::Text tileText(GameFont, "", TileSize);
	for (int y = 0; y < currentMap.GetHeight(); ++y)
	{
		for (int x = 0; x < currentMap.GetWidth(); ++x)
		{
			const Tile& tile = currentMap.GetTile(x, y);
			tileText.setString(std::string(1, tile.Glyph));
			tileText.setPosition({ static_cast<float>(x * TileSize), static_cast<float>(y * TileSize) });
			tileText.setFillColor(sf::Color::White);
			Window.draw(tileText);
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
