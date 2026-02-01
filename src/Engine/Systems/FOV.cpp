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

	// 원점 경계 체크 후 설정
	if (!InBounds(originX, originY))
	{
		return;
	}

	// 원점은 항상 보이게 설정
	SetVisible(originX, originY);

	// Shadowcasting - 8방향 (octants)
	static const int multipliers[4][8] = {
		{ 1, 0, 0, -1, -1, 0, 0, 1 },
		{ 0, 1, -1, 0, 0, -1, 1, 0 },
		{ 0, 1, 1, 0, 0, -1, -1, 0 },
		{ 1, 0, 0, 1, -1, 0, 0, -1 }
	};

	for (int oct = 0; oct < 8; ++oct)
	{
		CastLight(map, originX, originY, radius, 1, 1.0f, 0.0f,
			multipliers[0][oct], multipliers[1][oct],
			multipliers[2][oct], multipliers[3][oct]);
	}
}

void FOV::CastLight(const Map& map, int ox, int oy, int radius, int row, float startSlope, float endSlope, int xx, int xy, int yx, int yy)
{
	if (startSlope < endSlope)
	{
		return;
	}

	float nextStartSlope = startSlope;

	for (int i = row; i <= radius; ++i)
	{
		bool blocked = false;

		for (int dx = -i, dy = -i; dx <= 0; ++dx)
		{
			// 타일 경계 기준 슬로프 계산
			float lSlope = (dx - 0.5f) / (dy + 0.5f);
			float rSlope = (dx + 0.5f) / (dy - 0.5f);

			if (rSlope > startSlope)
			{
				continue;
			}
			if (lSlope < endSlope)
			{
				break;
			}

			// 옥탄트 변환
			int mapX = ox + dx * xx + dy * xy;
			int mapY = oy + dx * yx + dy * yy;

			if (!map.InBounds(mapX, mapY) || !InBounds(mapX, mapY))
			{
				continue;
			}

			float dist = std::sqrt(static_cast<float>(dx * dx + dy * dy));
			bool  inRadius = dist <= static_cast<float>(radius);

			const Tile& tile = map.GetTile(mapX, mapY);

			// 1. 그림자 상태 먼저 처리
			if (blocked)
			{
				if (!tile.Transparent)
				{
					// 그림자 안의 벽: 보임
					if (inRadius)
					{
						SetVisible(mapX, mapY);
					}
					nextStartSlope = rSlope;
				}
				else
				{
					// 그림자 안의 바닥: 안 보임, 그림자 종료
					blocked = false;
					startSlope = nextStartSlope;
				}

				continue;
			}

			// 2. 그림자가 아닌 경우: 보임
			if (inRadius)
			{
				SetVisible(mapX, mapY);
			}

			// 3. 벽이면 그림자 시작
			if (!tile.Transparent && i < radius)
			{
				blocked = true;
				CastLight(map, ox, oy, radius, i + 1, startSlope, rSlope, xx, xy, yx, yy);
				nextStartSlope = rSlope;
			}
		}

		if (blocked)
		{
			break;
		}
	}
}

void FOV::SetVisible(int x, int y)
{
	int idx = y * Width + x;
	Visible[idx] = true;
	Explored[idx] = true;
}

bool FOV::InBounds(int x, int y) const
{
	return x >= 0 && x < Width && y >= 0 && y < Height;
}

bool FOV::IsVisible(int x, int y) const
{
	if (x < 0 || x >= Width || y < 0 || y >= Height)
	{
		return false;
	}

	return Visible[y * Width + x];
}

bool FOV::IsExplored(int x, int y) const
{
	if (x < 0 || x >= Width || y < 0 || y >= Height)
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