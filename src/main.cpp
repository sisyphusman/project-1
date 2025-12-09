#include <SFML/Graphics.hpp>

int main()
{
	// 1920x1080 창 생성
	sf::RenderWindow window(sf::VideoMode({ 1920u, 1080u}), "project-1 with SFML");

	// 메인 루프
	while (window.isOpen())
	{
		// 이벤트 처리, pollEvent가 이벤트 반환
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

		// 랜더링
		window.clear();
		window.display();
	}

	return 0;
}