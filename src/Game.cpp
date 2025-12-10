#include "game.h"
#include "TimeManager.h"

Game::Game()
	: window_(sf::VideoMode({1920u, 1080u}), "project-1")
{
}

Game::~Game()
{
}

void Game::Run()
{
	// 메인 루프
	while (window_.isOpen())
	{
		ProcessEvents();
		TimeManager::GetInstance().Update();
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
		if (event->is<sf::Event::KeyPressed>())
		{
			const auto* keyEvent = event->getIf<sf::Event::KeyPressed>();
			if (keyEvent && keyEvent->code == sf::Keyboard::Key::Escape)
			{
				window_.close();
			}
		}
	}
}

void Game::Render()
{
	window_.clear();
	
	// TODO: 나중에 그릴 것

	window_.display();
}