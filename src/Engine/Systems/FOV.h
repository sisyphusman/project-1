#pragma once

#include <vector>

class Map;

// Bresenham Raycasting 알고리즘
class FOV
{
public:
	FOV(int width, int height);

	void Compute(const Map& map, int originX, int originY, int radius); // 주어진 원점과 반경 기준으로 시야 계산
	bool IsVisible(int x, int y) const;									// 특정 좌표가 현재 턴에 보이는지 확인
	bool IsExplored(int x, int y) const;								// 해당 좌표가 과거에 한 번이라도 보였는지 확인
	void Reset();

	// 탐험 데이터 저장/복원
	const std::vector<bool>& GetExploredData() const { return Explored; }
	void					 SetExploredData(const std::vector<bool>& data);

private:
	void CastRay(const Map& map, int originX, int originY, int targetX, int targetY, int radiusSquared); // Bresenham 광선 투사

	void SetVisible(int x, int y);	   // 해당 좌표는 보이게 설정
	bool InBounds(int x, int y) const; // 플레이어가 FOV 배열 범위 밖이면 중단

	int DistanceSquared(int x1, int y1, int x2, int y2) const; // 거리 제곱 계산 (sqrt 비용 방지)

	int				  Width;
	int				  Height;
	std::vector<bool> Visible;	// 현재 턴에서 보이는 타일
	std::vector<bool> Explored; // 한 번이라도 본 타일
};