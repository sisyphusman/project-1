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
- 괄호는 한 칸 띄우고 사용한다 ()

# Git Convention

- 타입: 항상 소문자
- 콜론 뒤 한 칸 띄우기

# Comment Guidelines

- source -> how it works, header -> what it does

# Include Order

0. 소스의 헤더
1. 표준 라이브러리
2. 외부 라이브러리
3. 프로젝트 내부 헤더  

- 그룹 간 공백 한 줄, 같은 그룹 내 알파벳 정렬

# File/Folder Name

- Github Project Title -> lowercase-kebab-case
- Window Files -> PascalCase