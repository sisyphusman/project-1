#include "Engine/Systems/FOV.h"

#include "Engine/World/Map.h"

#include <algorithm>
#include <cmath>

FOV::FOV(int width, int height)
	: Width(width)
	, Height(height)
	, Visible(width * height, false)
	, Explored(width * height, false)
{
}

void FOV::Compute(const Map& map, int originX, int originY, int radius)
{
	// 가시성 초기화
	std::fill(Visible.begin(), Visible.end(), false);

	// 원점 경계 체크
	if (!InBounds(originX, originY) || radius < 0)
	{
		return;
	}

	// 원점은 항상 보이게 설정
	SetVisible(originX, originY);

	if (radius == 0)
	{
		return;
	}

	const int radiusSquared = radius * radius;
	const int minX = std::max(0, originX - radius);
	const int maxX = std::min(Width - 1, originX + radius);
	const int minY = std::max(0, originY - radius);
	const int maxY = std::min(Height - 1, originY + radius);

	// 반경 내부의 모든 타일을 대상으로 광선 발사
	for (int y = minY; y <= maxY; ++y)
	{
		for (int x = minX; x <= maxX; ++x)
		{
			if (DistanceSquared(originX, originY, x, y) <= radiusSquared)
			{
				CastRay(map, originX, originY, x, y, radiusSquared);
			}
		}
	}
}

void FOV::CastRay(const Map& map, int originX, int originY, int targetX, int targetY, int radiusSquared)
{
	// 원점에서 목표까지 직선 경로의 모든 타일 검사 후 처리

	int dx = std::abs(targetX - originX);
	int dy = std::abs(targetY - originY);
	int sx = (originX < targetX) ? 1 : -1;
	int sy = (originY < targetY) ? 1 : -1;
	int err = dx - dy; // x or y 이동 결정

	int currentX = originX;
	int currentY = originY;

	// 매 루프마다 한 칸 이동
	while (true)
	{
		// 경계 체크
		if (!InBounds(currentX, currentY))
		{
			break;
		}

		// 반경 체크 (유클리드 거리)
		if (DistanceSquared(originX, originY, currentX, currentY) > radiusSquared)
		{
			break;
		}

		// 현재 타일 가시성 설정
		SetVisible(currentX, currentY);

		// 맵 경계 체크
		if (!map.InBounds(currentX, currentY))
		{
			break;
		}

		// 불투명 타일(벽)을 만나면 해당 타일은 보이지만 광선 중단
		const Tile& tile = map.GetTile(currentX, currentY);
		if (!tile.Transparent)
		{
			break;
		}

		// 목표 도달 확인
		if (currentX == targetX && currentY == targetY)
		{
			break;
		}

		// Bresenham 다음 스텝
		int e2 = 2 * err;

		if (e2 > -dy)
		{
			err -= dy;
			currentX += sx;
		}

		if (e2 < dx)
		{
			err += dx;
			currentY += sy;
		}
	}
}

void FOV::SetVisible(int x, int y)
{
	if (!InBounds(x, y))
	{
		return;
	}

	int idx = y * Width + x;
	Visible[idx] = true;
	Explored[idx] = true;
}

bool FOV::InBounds(int x, int y) const
{
	return x >= 0 && x < Width && y >= 0 && y < Height;
}

int FOV::DistanceSquared(int x1, int y1, int x2, int y2) const
{
	const int dx = x2 - x1;
	const int dy = y2 - y1;
	return dx * dx + dy * dy;
}

bool FOV::IsVisible(int x, int y) const
{
	if (!InBounds(x, y))
	{
		return false;
	}

	return Visible[y * Width + x];
}

bool FOV::IsExplored(int x, int y) const
{
	if (!InBounds(x, y))
	{
		return false;
	}

	return Explored[y * Width + x];
}

void FOV::Reset()
{
	std::fill(Visible.begin(), Visible.end(), false);
	std::fill(Explored.begin(), Explored.end(), false);
}

void FOV::SetExploredData(const std::vector<bool>& data)
{
	if (data.size() == Explored.size())
	{
		Explored = data;
	}
}