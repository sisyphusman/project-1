#include "engine/core/game.h"

Game::Game()
	: Window(sf::VideoMode({ 1920u, 1080u }), "project-1")
	, PlayerSpeed(200.f)
{
	Init();
}

Game::~Game()
{
	Clear();
}

void Game::Init()
{
	if (!PlayerFont.openFromFile("C:/Windows/Fonts/arial.ttf"))
	{
		return;
	}

	// 플레이어 설정 (@문자)
	PlayerText.emplace(PlayerFont, "@", 32);
	PlayerText->setFillColor(sf::Color::White);

	// 화면 중앙에 배치
	PlayerPos = { 640.f - 16.f, 360.f - 16.f };
	PlayerText->setPosition(PlayerPos);
}

void Game::Run()
{
	// 메인 루프
	while (Window.isOpen())
	{
		float delta_time = FrameClock.restart().asSeconds();

		ProcessEvents();
		Update(delta_time);
		Render();
	}
}

void Game::ProcessEvents()
{
	// 이벤트 처리
	// while문에서 변수 선언 하는 이유 1. 범위 최소화 2. 이전 값 참조 실수 방지
	while (const std::optional<sf::Event> event = Window.pollEvent())
	{
		// 창 닫기
		if (event->is<sf::Event::Closed>())
		{
			Window.close();
		}

		// 키 입력
		// getIf는 타입 확인 후 캐스팅
		if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>())
		{
			if (keyEvent->code == sf::Keyboard::Key::Escape)
			{
				Window.close();
			}
		}
	}
}

void Game::Update(float deltaTime)
{
	// 키보드 입력으로 이동
	sf::Vector2f dir{ 0.f, 0.f };

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
	{
		dir.y -= 1.f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
	{
		dir.y += 1.f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
	{
		dir.x -= 1.f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
	{
		dir.x += 1.f;
	}

	// 대각선 이동 정규화
	if (dir.x != 0.f || dir.y != 0.f)
	{
		float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);	// 벡터의 길이 계산
		dir /= len;												// 벡터를 길이로 나눠서 단위 벡터로 변환
	}

	// 위치 업데이트
	PlayerPos += dir * PlayerSpeed * deltaTime;

	if (PlayerText)
	{
		PlayerText->setPosition(PlayerPos);
	}
}

void Game::Render()
{
	Window.clear();

	Window.draw(*PlayerText);

	Window.display();
}

void Game::Clear()
{
}