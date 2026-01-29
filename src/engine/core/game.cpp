#include "engine/core/game.h"

Game::Game()
	: window_(sf::VideoMode({ 1920u, 1080u }), "project-1")
{
	Init();
}

Game::~Game()
{
	Clear();
}

void Game::Init()
{
	// 플레이어 설정 (32x32 흰색 사각형)
	if (!player_font_.openFromFile("C:/Windows/Fonts/arial.ttf"))
	{
		return;
	}

	// 플레이어 설정 (@문자)
	player_.emplace(player_font_, "@", 32);
	player_->setFillColor(sf::Color::White);

	// 화면 중앙에 배치
	player_pos_ = { 640.f - 16.f, 360.f - 16.f };
	player_->setPosition(player_pos_);
}

void Game::Run()
{
	// 메인 루프
	while (window_.isOpen())
	{
		float delta_time = clock_.restart().asSeconds();

		ProcessEvents();
		Update(delta_time);
		Render();
	}
}

void Game::ProcessEvents()
{
	// 이벤트 처리
	// while문에서 변수 선언 하는 이유 1. 범위 최소화 2. 이전 값 참조 실수 방지
	while (const std::optional<sf::Event> event = window_.pollEvent())
	{
		// 창 닫기
		if (event->is<sf::Event::Closed>())
		{
			window_.close();
		}

		// 키 입력
		// getIf는 타입 확인 후 캐스팅
		if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>())
		{
			if (keyEvent->code == sf::Keyboard::Key::Escape)
			{
				window_.close();
			}
		}
	}
}

void Game::Update(float delta_time)
{
	// 키보드 입력으로 이동
	sf::Vector2f movement{ 0.f, 0.f };

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
	{
		movement.y -= 1.f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
	{
		movement.y += 1.f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
	{
		movement.x -= 1.f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
	{
		movement.x += 1.f;
	}

	// 위치 업데이트
	player_pos_ += movement * player_speed_ * delta_time;

	if (player_)
	{
		player_->setPosition(player_pos_);
	}
}

void Game::Render()
{
	window_.clear();

	window_.draw(*player_);

	window_.display();
}

void Game::Clear()
{
}