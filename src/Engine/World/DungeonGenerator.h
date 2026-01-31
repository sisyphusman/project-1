#pragma once

#include <vector>
#include <random>

class Map;

struct Room
{
	int X, Y, Width, Height;

	int CenterX() const { return X + Width / 2; }
	int CenterY() const { return Y + Height / 2; }
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
	void CreateHTunnel(Map& map, int x1, int x2, int y); // Horizontal (수평)
	void CreateVTunnel(Map& map, int y1, int y2, int x); // Vertical (수직)

	std::mt19937	  Rng;
	std::vector<Room> Rooms;
};