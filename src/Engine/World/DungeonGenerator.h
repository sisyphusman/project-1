#pragma once

#include <vector>
#include <random>
#include <queue>
#include <unordered_set>

#include <SFML/System/Vector2.hpp>

class Map;

struct Room
{
	int X, Y, Width, Height; // X, Y: 방의 시작 좌표

	int CenterX() const { return X + Width / 2; }
	int CenterY() const { return Y + Height / 2; }

	// 점이 방 내부에 있는지 확인
	bool Contains(int px, int py) const
	{
		return px >= X && px < X + Width && py >= Y && py < Y + Height;
	}
};

// A* 경로찾기용 노드
struct PathNode
{
	int X, Y;
	int G, H; // G: 시작점부터 비용, H: 목표까지 휴리스틱
	int F() const { return G + H; }

	bool operator>(const PathNode& other) const { return F() > other.F(); }
};

// BSP(Binary Space Partitioning) 기반 랜덤 던전 생성기
class DungeonGenerator
{
public:
	DungeonGenerator(unsigned int seed = 0);

	void					 Generate(Map& map, int maxRooms, int roomMinSize, int roomMaxSize);
	const std::vector<Room>& GetRooms() const { return Rooms; }

private:
	void CreateRoom(Map& map, const Room& room);

	void					  CreatePathBetweenRooms(Map& map, const Room& from, const Room& to);	// A* 기반 통로 생성
	std::vector<sf::Vector2i> FindPath(const Map& map, int startX, int startY, int endX, int endY); // A* 경로 찾기

	// 타일이 통로로 사용 가능한지 (방 내부가 아닌 곳)
	bool IsValidForCorridor(int x, int y, int endX, int endY) const;

	int Heuristic(int x1, int y1, int x2, int y2) const;

	std::mt19937	  Rng;
	std::vector<Room> Rooms;

	// A* 탐색 시 맵 크기 캐싱
	int MapWidth = 0;
	int MapHeight = 0;
};