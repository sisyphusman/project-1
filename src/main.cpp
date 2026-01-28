#include "game.h"

// main.cpp는 클래스가 아니고 일반 파일이라 소문자 이름 사용

int main()
{
	Game game;
	game.Run();

	return 0;
}

// Code Convention
// Google Style + C++ Core GuideLine

// Class, Struct, Enum, Method -> PascalCase
// Local Var, File Name -> snake_case
// Member Var -> snake_case_
// Constant, Enum Value -> kPascalCase
// Namespace -> snake_case
// Macro -> UPPER_SNAKE_CASE