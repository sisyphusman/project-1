#include "Engine/Core/Game.h"

int main()
{
	Game game;
	game.Run();

	return 0;
}

// TODO
// 1. 플레이어 사망 처리
// 2. 메인 메뉴, 게임 메뉴 만들기
// 3. 대각선 이동
// 4. 최적화(Game.cpp 리펙토링, draw 콜 줄이기, 렌더링 기법 변경)

// 가장 아름다운 세계는 자신의 상상력으로 만들어낸 세계다