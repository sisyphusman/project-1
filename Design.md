# Code Convention

- Unreal Style + Unreal Clang Format
```
Class / Struct / Enum / Interface -> PascalCase (with prefix U/A/F/E/I)
Function / Method                 -> PascalCase
Member Variable                   -> PascalCase
Local Variable / Parameter        -> camelCase
bool Variable                     -> bPrefix + PascalCase
Constant                          -> PascalCase
Enum Value                        -> PascalCase
Namespace                         -> 거의 사용 안 함
File Name                         -> PascalCase
Macro                             -> UPPER_SNAKE_CASE
```

- 블록 단위일때 중괄호는 새로운 줄에서 시작, return에서 중괄호는 블록이 아니므로 제외

# Comment Guidelines

- source -> how it works, header -> what it does

# Include Order
0. 소스의 헤더
1. 표준 라이브러리
2. 외부 라이브러리
3. 프로젝트 내부 헤더  
- 그룹 간 공백 한 줄, 같은 그룹 내 알파벳 정렬

# Architecture

```
src/
├── core/
├── actors/
├── items/
├── world/
├── systems/
├── ui/
├── audio/
├── data/
├── events/
└── main.cpp

assets/
tools/
```

# Game Design
 1. 게임은 랜덤을 기반으로 설계되었지만 실력이 큰 차이를 만들게 한다 
 2. 세이브-로드가 없고 목숨은 한개이다 
 3. 모든 조작은 키보드만으로 가능하게 하고 단축키를 최소화한다 
 4. 정해진 공략이 최대한 없도록 만든다
 5. 스토리와 세계관도 핵심 재미 요소이고, 읽어야 할 텍스트는 최소화하여 피로를 줄인다