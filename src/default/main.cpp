#include "engine/core/game.h"

int main()
{
	Game game;
	game.Run();

	return 0;
}

// ----- Code Convention -----

// Unreal Style + Unreal Clang Format

// Class / Struct / Enum / Interface -> PascalCase (with prefix U/A/F/E/I)
// Function / Method                 -> PascalCase
// Member Variable                   -> PascalCase
// Local Variable / Parameter        -> camelCase
// bool Variable                     -> bPrefix + PascalCase
// Constant                          -> PascalCase
// Enum Value                        -> PascalCase
// Namespace                         -> 거의 사용 안 함
// File Name                         -> PascalCase
// Macro                             -> UPPER_SNAKE_CASE

// 블록 단위일때 중괄호는 새로운 줄에서 시작, return에서 중괄호는 블록이 아니므로 제외

// ----- Comment Guidelines -----
// source -> how it works, header -> what it does

// 가장 아름다운 세계는 자신의 상상력으로 만들어낸 세계다