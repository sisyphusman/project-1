#pragma once

class Camera
{
public:
	Camera() = default;

	void SetTarget(float x, float y);
	void ToggleZoomOut();
	void Update(float deltaTime); // 지수 보간(exponential smoothing) 사용

	[[nodiscard]] float GetX() const noexcept { return m_x; }
	[[nodiscard]] float GetY() const noexcept { return m_y; }
	[[nodiscard]] float GetZoom() const noexcept { return m_currentZoom; }
	[[nodiscard]] bool	IsZoomedOut() const noexcept { return m_isZoomedOut; }

	void SetNormalZoom(float zoom) { m_normalZoom = zoom; }
	void SetZoomedOutZoom(float zoom) { m_zoomedOutZoom = zoom; }

private:
	// 현재 카메라 중심 좌표
	float m_x = 0.0f;
	float m_y = 0.0f;

	// 목표 좌표
	float m_targetX = 0.0f;
	float m_targetY = 0.0f;

	float m_normalZoom = 1.0f;	  // 기본
	float m_zoomedOutZoom = 0.5f; // 줌 아웃
	float m_currentZoom = 1.0f;	  // 보간 결과

	bool m_isZoomedOut = false;

	// 지수 보간 계수, 값이 크면 빠름
	static constexpr float SmoothSpeed = 8.0f;
	static constexpr float ZoomSpeed = 5.0f;
};