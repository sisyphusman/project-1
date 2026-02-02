#include "Engine/Systems/FOV.h"

#include "Engine/World/Map.h"

#include <cmath>
#include <algorithm>

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
	if (!InBounds(originX, originY))
	{
		return;
	}

	// 원점은 항상 보이게 설정
	SetVisible(originX, originY);

	// 시야 원의 경계점들 계산
	std::vector<std::pair<int, int>> circlePoints;
	GetCirclePoints(originX, originY, radius, circlePoints);

	// 각 경계점으로 광선 발사
	for (const auto& [targetX, targetY] : circlePoints)
	{
		CastRay(map, originX, originY, targetX, targetY, radius);
	}
}

void FOV::GetCirclePoints(int centerX, int centerY, int radius, std::vector<std::pair<int, int>>& points)
{
	// 모든 분면(octant)의 점을 수집하여 완전한 원 형성

	int x = radius;
	int y = 0;
	int radiusError = 1 - x; // 원 내부/외부 판단 기준

	while (x >= y)
	{
		// 8분면 대칭점 추가
		points.emplace_back(centerX + x, centerY + y);
		points.emplace_back(centerX + y, centerY + x);
		points.emplace_back(centerX - y, centerY + x);
		points.emplace_back(centerX - x, centerY + y);
		points.emplace_back(centerX - x, centerY - y);
		points.emplace_back(centerX - y, centerY - x);
		points.emplace_back(centerX + y, centerY - x);
		points.emplace_back(centerX + x, centerY - y);

		++y;

		// 원 안쪽이면 x 유지, 오차 보정
		if (radiusError < 0)
		{
			radiusError += 2 * y + 1;
		}
		// 원 바깥쪽이면 x 감소 후, 오차 보정
		else
		{
			--x;
			radiusError += 2 * (y - x + 1);
		}
	}

	std::sort(points.begin(), points.end());
	points.erase(std::unique(points.begin(), points.end()), points.end()); // 연속된 중복 제거
}

void FOV::CastRay(const Map& map, int originX, int originY, int targetX, int targetY, int radius)
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
		float dist = Distance(originX, originY, currentX, currentY);
		if (dist > static_cast<float>(radius))
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

float FOV::Distance(int x1, int y1, int x2, int y2) const
{
	float dx = static_cast<float>(x2 - x1);
	float dy = static_cast<float>(y2 - y1);
	return std::sqrt(dx * dx + dy * dy);
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