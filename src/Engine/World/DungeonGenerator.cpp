#include "Engine/World/DungeonGenerator.h"

#include <algorithm>
#include <unordered_map>

#include "Engine/World/Map.h"

DungeonGenerator::DungeonGenerator(unsigned int seed)
{
	if (seed == 0)
	{
		// 시드용 정수값을 만들 수 있는 객체 생성
		std::random_device rd;

		// 정수값으로 난수 엔진 초기화
		Rng.seed(rd());
	}
	else
	{
		Rng.seed(seed);
	}
}

void DungeonGenerator::Generate(Map& map, int maxRooms, int roomMinSize, int roomMaxSize)
{
	Rooms.clear();
	MapWidth = map.GetWidth();
	MapHeight = map.GetHeight();

	// 맵 전체를 벽으로 채우기
	for (int y = 0; y < MapHeight; ++y)
	{
		for (int x = 0; x < MapWidth; ++x)
		{
			map.SetTile(x, y, Tile::Wall());
		}
	}

	std::uniform_int_distribution<int> widthDist(roomMinSize, roomMaxSize);
	std::uniform_int_distribution<int> heightDist(roomMinSize, roomMaxSize);

	for (int i = 0; i < maxRooms; ++i)
	{
		// w, h의 범위는 [min,max]
		int w = widthDist(Rng);
		int h = heightDist(Rng);

		// 방이 상하좌우 경계선을 침범하지 않음
		std::uniform_int_distribution<int> xDist(1, MapWidth - w - 1);
		std::uniform_int_distribution<int> yDist(1, MapHeight - h - 1);

		Room newRoom(xDist(Rng), yDist(Rng), w, h);

		// 다른 방과 겹치는지 확인 (AABB 충돌 검사)
		bool overlaps = false;
		int	 padding = 2;
		for (const auto& other : Rooms)
		{
			if (newRoom.X <= other.X + other.Width + padding && newRoom.X + newRoom.Width + padding >= other.X && newRoom.Y <= other.Y + other.Height + padding && newRoom.Y + newRoom.Height + padding >= other.Y)
			{
				overlaps = true;
				break;
			}
		}

		if (overlaps)
		{
			continue;
		}

		// 방 생성
		CreateRoom(map, newRoom);

		// 첫 번째 방이 아니면 이전 방과 연결
		if (!Rooms.empty())
		{
			CreatePathBetweenRooms(map, Rooms.back(), newRoom);
		}

		// 방 목록에 추가, 다음 방의 연결 기준점
		Rooms.push_back(newRoom);
	}
}

void DungeonGenerator::CreateRoom(Map& map, const Room& room)
{
	for (int y = room.Y; y < room.Y + room.Height; ++y)
	{
		for (int x = room.X; x < room.X + room.Width; ++x)
		{
			map.SetTile(x, y, Tile::Floor());
		}
	}
}

void DungeonGenerator::CreatePathBetweenRooms(Map& map, const Room& from, const Room& to)
{
	auto path = FindPath(map, from.CenterX(), from.CenterY(), to.CenterX(), to.CenterY());

	// 경로를 따라 바닥 타일 생성
	for (const auto& pos : path)
	{
		map.SetTile(pos.x, pos.y, Tile::Floor());
	}
}

std::vector<sf::Vector2i> DungeonGenerator::FindPath(const Map& map, int startX, int startY, int endX, int endY)
{
	// 우선순위 큐 (F 비용이 낮은 것부터)
	std::priority_queue<PathNode, std::vector<PathNode>, std::greater<PathNode>> openList;

	// 방문 체크 ( y * width + x를 키로 사용)
	std::unordered_set<int> closedSet;

	// 부모 노드 추적 (경로 복원용)
	std::unordered_map<int, sf::Vector2i> cameFrom;

	auto toIndex = [this](int x, int y) { return y * MapWidth + x; };

	// 시작 노드 추가
	openList.push({ startX, startY, 0, Heuristic(startX, startY, endX, endY) });

	// 4방향 이동
	const int dx[] = { 0, 0, -1, 1 };
	const int dy[] = { -1, 1, 0, 0 };

	while (!openList.empty())
	{
		PathNode current = openList.top();
		openList.pop();

		int currentIndex = toIndex(current.X, current.Y);

		// 이미 방문한 노드면 스킵
		if (closedSet.count(currentIndex))
		{
			continue;
		}

		closedSet.insert(currentIndex);

		// 목표 도달
		if (current.X == endX && current.Y == endY)
		{
			// 경로 복원
			std::vector<sf::Vector2i> path;
			sf::Vector2i			  pos{ endX, endY };

			while (!(pos.x == startX && pos.y == startY))
			{
				path.push_back(pos);
				int idx = toIndex(pos.x, pos.y);
				if (cameFrom.find(idx) == cameFrom.end())
				{
					break;
				}
				pos = cameFrom[idx];
			}
			path.push_back({ startX, startY });

			std::reverse(path.begin(), path.end());
			return path;
		}

		// 인접 노드 탐색
		for (int i = 0; i < 4; ++i)
		{
			int nx = current.X + dx[i];
			int ny = current.Y + dy[i];

			// 맵 경계 체크
			if (nx < 1 || nx >= MapWidth - 1 || ny < 1 || ny >= MapHeight - 1)
			{
				continue;
			}

			int neighborIndex = toIndex(nx, ny);

			// 이미 방문했으면 스킵
			if (closedSet.count(neighborIndex))
			{
				continue;
			}

			// 비용 계산 (기존 방 내부는 높은 비용 부여)
			int moveCost = 1;
			if (IsValidForCorridor(nx, ny, endX, endY) == false)
			{
				moveCost = 100;
			}

			int newG = current.G + moveCost;
			int newH = Heuristic(nx, ny, endX, endY);

			cameFrom[neighborIndex] = { current.X, current.Y };
			openList.push({ nx, ny, newG, newH });
		}
	}

	// 경로를 찾지 못한 경우 L자 통로로 폴백
	std::vector<sf::Vector2i> fallbackPath;

	int x = startX;
	int y = startY;

	// 수평 이동
	while (x != endX)
	{
		fallbackPath.push_back({ x, y });
		x += (endX > x) ? 1 : -1;
	}

	// 수직 이동
	while (y != endY)
	{
		fallbackPath.push_back({ x, y });
		y += (endY > y) ? 1 : -1;
	}

	fallbackPath.push_back({ endX, endY });
	return fallbackPath;
}

bool DungeonGenerator::IsValidForCorridor(int x, int y, int endX, int endY) const
{
	for (const auto& room : Rooms)
	{
		// 시작/끝 방문 제외
		if (room.Contains(endX, endY))
		{
			continue;
		}

		if (room.Contains(x, y))
		{
			return false;
		}
	}

	return true;
}

int DungeonGenerator::Heuristic(int x1, int y1, int x2, int y2) const
{
	return std::abs(x1 - x2) + std::abs(y1 - y2);
}