#include "engine/core/game.h"

// main.cpp는 클래스가 아니고 일반 파일이라 소문자 이름 사용

int main()
{
	Game game;
	game.Run();

	return 0;
}

// ----- Code Convention -----

// Google Style + C++ Core GuideLine + Visual Studio Formatting Style

// Class, Struct, Enum, Method -> PascalCase
// Local Var, File Name -> snake_case
// Member Var -> snake_case_
// Constant, Enum Value -> kPascalCase
// Namespace -> snake_case
// Macro -> UPPER_SNAKE_CASE

// ----- Comment Convention -----
// cpp -> how?, header -> what?

// 가장 아름다운 세계는 자신의 상상력으로 만들어낸 세계다