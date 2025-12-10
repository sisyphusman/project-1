#include "game.h"

Game::Game()
	: window(sf::VideoMode({1920u, 1080u}), "project-1")
{
}

Game::~Game()
{
}

void Game::run()
{
	// 메인 루프
	while (window.isOpen())
	{
		processEvents();
		render();
	}
}

void Game::processEvents()
{
	// 이벤트 처리
	// while문에서 변수 선언 하는 이유 1. 범위 최소화 2. 이전 값 참조 실수 방지
	while (const std::optional<sf::Event> event = window.pollEvent())
	{
		// 창 닫기
		if (event->is<sf::Event::Closed>())
		{
			window.close();
		}

		// 키 입력
		if (event->is<sf::Event::KeyPressed>())
		{
			const auto* keyEvent = event->getIf<sf::Event::KeyPressed>();
			if (keyEvent && keyEvent->code == sf::Keyboard::Key::Escape)
			{
				window.close();
			}
		}
	}
}

void Game::render()
{
	window.clear();
	
	// TODO: 나중에 그릴 것

	window.display();
}