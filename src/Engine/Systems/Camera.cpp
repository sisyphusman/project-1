#include "Engine/Systems/Camera.h"

#include <algorithm>
#include <cmath>

void Camera::SetTarget(float x, float y)
{
	m_targetX = x;
	m_targetY = y;
}

void Camera::ToggleZoomOut()
{
	m_isZoomedOut = !m_isZoomedOut;
}

void Camera::Update(float deltaTime, sf::View& inOutView, float baseViewWidth, float baseViewHeight)
{
	// 부드러운 카메라 추적, 보간 계수 t	
	const float t = 1.0f - std::exp(-SmoothSpeed * deltaTime);
	m_x += (m_targetX - m_x) * t;
	m_y += (m_targetY - m_y) * t;

	// 부드러운 줌 전환
	const float targetZoom = m_isZoomedOut ? m_zoomedOutZoom : m_normalZoom;
	const float zoomT = 1.0f - std::exp(-ZoomSpeed * deltaTime);
	m_currentZoom += (targetZoom - m_currentZoom) * zoomT;

	// 카메라 보간 결과를 즉시 뷰에 반영
	const float safeZoom = std::max(0.001f, m_currentZoom);
	inOutView.setSize({ baseViewWidth / safeZoom, baseViewHeight / safeZoom });
	inOutView.setCenter({ m_x, m_y });
}