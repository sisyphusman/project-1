#pragma once

#include <SFML/Graphics/View.hpp>

class Camera
{
public:
	Camera() = default;

	void SetTarget(float x, float y);
	void ToggleZoomOut();
	void Update(float deltaTime, sf::View& inOutView, float baseViewWidth, float baseViewHeight); // 지수 보간 + 뷰 반영

	[[nodiscard]] float GetX() const noexcept { return CurrentX; }
	[[nodiscard]] float GetY() const noexcept { return CurrentY; }
	[[nodiscard]] float GetZoom() const noexcept { return CurrentZoom; }
	[[nodiscard]] bool	IsZoomedOut() const noexcept { return bIsZoomedOut; }

	void SetNormalZoom(float zoom) { NormalZoom = zoom; }
	void SetZoomedOutZoom(float zoom) { ZoomedOutZoom = zoom; }

private:
	// 현재 카메라 중심 좌표
	float CurrentX = 0.0f;
	float CurrentY = 0.0f;

	// 목표 좌표
	float TargetX = 0.0f;
	float TargetY = 0.0f;

	float NormalZoom = 1.0f;	// 기본
	float ZoomedOutZoom = 0.5f; // 줌 아웃
	float CurrentZoom = 1.0f;	// 보간 결과

	bool bIsZoomedOut = false;

	// 지수 보간 계수, 값이 크면 빠름
	static constexpr float SmoothSpeed = 8.0f;
	static constexpr float ZoomSpeed = 5.0f;
};