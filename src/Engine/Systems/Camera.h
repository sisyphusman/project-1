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
	[[nodiscard]] float GetZoom() const noexcept { return CurrentScale; }
	[[nodiscard]] bool	IsZoomedOut() const noexcept { return ZoomedOut; }

	void SetNormalZoom(float zoom) { NormalScale = zoom; }
	void SetZoomedOutZoom(float zoom) { ZoomedOutScale = zoom; }

private:
	// 현재 카메라 중심 좌표
	float CurrentX = 0.0f;
	float CurrentY = 0.0f;

	// 목표 좌표
	float TargetX = 0.0f;
	float TargetY = 0.0f;

	float NormalScale = 1.0f;	// 기본
	float ZoomedOutScale = 0.5f; // 줌 아웃
	float CurrentScale = 1.0f;	// 보간 결과

	bool ZoomedOut = false;

	// 지수 보간 계수, 값이 크면 빠름
	static constexpr float SmoothSpeed = 8.0f;
	static constexpr float ZoomSpeed = 5.0f;
};