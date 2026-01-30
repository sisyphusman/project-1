#include "Engine/Core/Game.h"
#include "Engine/World/Map.h"
#include "Engine/Entities/Player.h"

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

	// 맵 설정 (@문자)
	GameMap = std::make_unique<Map>(ViewWidthTiles, ViewHeightTiles);

	// 플레이어 생성 (맵 중앙)
	GamePlayer = std::make_unique<Player>(ViewWidthTiles / 2, ViewHeightTiles / 2);
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
		// 창 닫기
		if (event->is<sf::Event::Closed>())
		{
			Window.close();
		}

		// 턴 기반
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
				default:
					break;
			}

			if (dx != 0 || dy != 0)
			{
				GamePlayer->TryMove(dx, dy, *GameMap);
			}
		}
	}
}

void Game::Update()
{
}

void Game::Render()
{
	Window.clear(sf::Color::Black);

	// 맵 랜더링
	sf::Text tileText(GameFont, "", TileSize);
	for (int y = 0; y < GameMap->GetHeight(); ++y)
	{
		for (int x = 0; x < GameMap->GetWidth(); ++x)
		{
			const Tile& tile = GameMap->GetTile(x, y);
			tileText.setString(std::string(1, tile.Glyph));
			tileText.setPosition({ static_cast<float>(x * TileSize), static_cast<float>(y * TileSize) });
			tileText.setFillColor(tile.Walkable ? sf::Color(100, 100, 100) : sf::Color(50, 50, 50));
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