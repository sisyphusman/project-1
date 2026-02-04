#pragma once

#include <cstdlib>
#include <iostream>

// 치명적 오류, abort 실행
// 매크로를 하나의 문단으로 만들기 위해서 do while 사용
#define GAME_CHECK(expr)                                                                              \
	do                                                                                                \
	{                                                                                                 \
		if (!(expr))                                                                                  \
		{                                                                                             \
			std::cerr << "오류: " #expr << " 파일:" << __FILE__ << " 라인:" << __LINE__ << std::endl;  \
			std::abort();                                                                             \
		}                                                                                             \
	}                                                                                                 \
	while (0)

// 디버그에서만 체크, 릴리즈 모드이면 생략
#ifdef _DEBUG
	#define GAME_CHECK_SLOW(expr) GAME_CHECK(expr)
#else
	#define GAME_CHECK_SLOW(expr) ((void)0)
#endif

// 경고, 계속 진행, 매크로를 함수처럼 안전하게 쓰기 위해 람다 사용
#define GAME_ENSURE(expr)																			  \
	([&]() -> bool																					  \
    {																								  \
		if (!(expr))                                                                                  \
		{                                                                                             \
			std::cerr << "경고: " #expr << " 파일:" << __FILE__ << " 라인:" << __LINE__ << std::endl;  \
			return false;                                                                             \
		}                                                                                             \
		return true;                                                                                  \
    })()																							  \