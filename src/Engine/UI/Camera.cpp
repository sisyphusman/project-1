#include "Camera.h"

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

void Camera::Update(float deltaTime)
{
	// 부드러운 카메라 추적, 보간 계수 t	
	const float t = 1.0f - std::exp(-SmoothSpeed * deltaTime);
	m_x += (m_targetX - m_x) * t;
	m_y += (m_targetY - m_y) * t;

	// 부드러운 줌 전환
	const float targetZoom = m_isZoomedOut ? m_zoomedOutZoom : m_normalZoom;
	const float zoomT = 1.0f - std::exp(-ZoomSpeed * deltaTime);
	m_currentZoom += (targetZoom - m_currentZoom) * zoomT;
}