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
}

void Game::Render()
{
	window_.clear();

	// TODO: 나중에 그릴 것

	window_.display();
}

void Game::Clear()
{

}