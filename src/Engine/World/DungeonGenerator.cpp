#include "Engine/World/DungeonGenerator.h"
#include "Engine/World/Map.h"

DungeonGenerator::DungeonGenerator(unsigned int seed)
{
	if (seed == 0)
	{
		// std::random_device는 복사가 금지, 임시 객체로 바로 호출하면 에러
		std::random_device rd;
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

	// 맵 전체를 벽으로 채우기
	for (int y = 0; y < map.GetHeight(); ++y)
	{
		for (int x = 0; x < map.GetWidth(); ++x)
		{
			map.GetTile(x, y) = Tile::Wall();
		}
	}

	std::uniform_int_distribution<int> widthDist(roomMinSize, roomMaxSize);
	std::uniform_int_distribution<int> heightDist(roomMinSize, roomMaxSize);

	for (int i = 0; i < maxRooms; ++i)
	{
		int w = widthDist(Rng);
		int h = heightDist(Rng);

		// 방이 상하좌우 경계선을 침범하지 않음
		std::uniform_int_distribution<int> xDist(1, map.GetWidth() - w - 1);
		std::uniform_int_distribution<int> yDist(1, map.GetHeight() - h - 1);

		Room newRoom(xDist(Rng), yDist(Rng), w, h);

		// 다른 방과 겹치는지 확인 (AABB 충돌 검사)
		bool overlaps = false;
		for (const auto& other : Rooms)
		{
			if (newRoom.X <= other.X + other.Width + 1 && newRoom.X + newRoom.Width + 1 >= other.X && newRoom.Y <= other.Y + other.Height + 1 && newRoom.Y + newRoom.Height + 1 >= other.Y)
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
			const Room& prev = Rooms.back();

			// 방 중심점끼리 L자 형태 통로 연결, 50% 확률로 가로 혹은 세로 연결
			if (Rng() % 2 == 0)
			{
				CreateHTunnel(map, prev.CenterX(), newRoom.CenterX(), prev.CenterY());
				CreateVTunnel(map, prev.CenterY(), newRoom.CenterY(), newRoom.CenterX());
			}
			else
			{
				CreateVTunnel(map, prev.CenterY(), newRoom.CenterY(), prev.CenterX());
				CreateHTunnel(map, prev.CenterX(), newRoom.CenterX(), newRoom.CenterY());
			}
		}

		// 방 목록에 추가, 다음 방의 연결 기준점^
		Rooms.push_back(newRoom);
	}
}

void DungeonGenerator::CreateRoom(Map& map, const Room& room)
{
	for (int y = room.Y; y < room.Y + room.Height; ++y)
	{
		for (int x = room.X; x < room.X + room.Width; ++x)
		{
			map.GetTile(x, y) = Tile::Floor();
		}
	}
}

void DungeonGenerator::CreateHTunnel(Map& map, int x1, int x2, int y)
{
	int start = std::min(x1, x2);
	int end = std::max(x1, x2);

	// Wall -> Floor로 변경
	for (int x = start; x <= end; ++x)
	{
		map.GetTile(x, y) = Tile::Floor();
	}
}

void DungeonGenerator::CreateVTunnel(Map& map, int y1, int y2, int x)
{
	int start = std::min(y1, y2);
	int end = std::max(y1, y2);

	for (int y = start; y <= end; ++y)
	{
		map.GetTile(x, y) = Tile::Floor();
	}
}