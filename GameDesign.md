
# Architecture

```
src/
├── Core/
├── Actors/
├── Items/
├── World/
├── Systems/
├── UI/
├── Audio/
├── Data/
├── Events/
└── Main.cpp

assets/
tools/
```

# Game Design

 1. 게임은 랜덤을 기반으로 설계되었지만 실력이 큰 차이를 만들게 한다
 2. 세이브-로드가 없고 목숨은 한개이다
 3. 모든 조작은 키보드만으로 가능하게 하고 단축키를 최소화한다
 4. 정해진 공략이 최대한 없도록 만든다
 5. 스토리와 세계관도 핵심 재미 요소이고, 읽어야 할 텍스트는 최소화하여 피로를 줄인다